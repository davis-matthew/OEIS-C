/*
 * A095292  Number of A095282-primes in range ]2^n,2^(n+1)].
 * https://oeis.org/A095292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p in (2^n, 2^(n+1)] whose binary
 *            expansion ends in an even number (>=2) of 1's, i.e. p == 2^k-1
 *            (mod 2^(k+1)) for even k>=2; computed by segmented sieve over
 *            each residue class
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static int64_t A095292_inv(int64_t a, int64_t m)
{
    int64_t t = 0, nt = 1, r = m, nr = a % m;
    while (nr != 0) {
        int64_t q = r / nr, tmp;
        tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += m;
    return t;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t L = (uint64_t)1 << n;
    uint64_t H = L << 1;
    uint64_t lim = 1;
    while (lim * lim < H) lim++;

    /* odd primes up to lim */
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 2, 1);
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 4));
    size_t np = 0;
    for (uint64_t i = 3; i <= lim; i += 2) {
        if (!comp[i]) {
            pr[np++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += 2 * i) comp[j] = 1;
        }
    }
    free(comp);

    const size_t SEG = (size_t)1 << 18;
    unsigned char *seg = (unsigned char *)malloc(SEG);
    uint64_t *nx = (uint64_t *)malloc(sizeof(uint64_t) * (np + 1));
    uint64_t total = 0;

    for (unsigned k = 2; k < 64; k += 2) {
        uint64_t r = ((uint64_t)1 << k) - 1;
        if (r > H) break;
        uint64_t M = (uint64_t)1 << (k + 1);
        uint64_t mlo = (L >= r) ? (L - r) / M + 1 : 0;
        uint64_t mhi = (H - r) / M;
        if (mlo > mhi) continue;
        for (size_t i = 0; i < np; i++) {
            uint64_t q = pr[i];
            uint64_t rm = r % q;
            uint64_t c = ((q - rm) % q) * (uint64_t)A095292_inv((int64_t)(M % q), (int64_t)q) % q;
            uint64_t s = mlo % q;
            nx[i] = (c + q - s) % q;
        }
        for (uint64_t s = mlo; s <= mhi; s += SEG) {
            uint64_t len = mhi - s + 1;
            if (len > SEG) len = SEG;
            for (size_t i = 0; i < len; i++) seg[i] = 1;
            for (size_t i = 0; i < np; i++) {
                uint64_t q = pr[i];
                uint64_t j = nx[i];
                for (; j < len; j += q) seg[j] = 0;
                nx[i] = j - len;
            }
            for (size_t i = 0; i < len; i++) total += seg[i];
        }
    }
    free(seg);
    free(nx);
    free(pr);
    return (seq_t)total;
}
