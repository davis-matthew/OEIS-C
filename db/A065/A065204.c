/*
 * A065204  Least m such that A065203(m) = n.
 * https://oeis.org/A065204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = k-1 where k is the least index with A064413(k) =
 *            A002110(n) (primorial); computed by direct EKG simulation
 *            (search)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t A065204_a(idx_t n)
{
    static const uint32_t pr[10] = {2,3,5,7,11,13,17,19,23,29};
    uint64_t P = 1;
    idx_t i;
    if (n < 1 || n > 9) return (seq_t)0;
    for (i = 0; i < n; i++) P *= pr[i];
    uint64_t lim = 2 * P + 100;
    if (lim > 500000000ULL) return (seq_t)0;
    uint32_t L = (uint32_t)lim;

    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint32_t *ptr = (uint32_t *)malloc(((size_t)L + 1) * sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc((size_t)L + 1, 1);
    uint32_t *primes = (uint32_t *)malloc(sizeof(uint32_t) * (L / 8 + 1000));
    if (!spf || !ptr || !used || !primes) exit(1);
    size_t pc = 0;
    for (uint32_t x = 2; x <= L; x++) {
        if (spf[x] == 0) { spf[x] = x; primes[pc++] = x; ptr[x] = x; }
        for (size_t j = 0; j < pc; j++) {
            uint32_t p = primes[j];
            uint64_t m = (uint64_t)x * p;
            if (p > spf[x] || m > L) break;
            spf[m] = p;
        }
    }
    used[1] = 1; used[2] = 1;
    uint32_t prev = 2;
    uint64_t k = 2;
    uint64_t res = 0;
    if (P == 2) res = 1;
    while (!res) {
        uint32_t x = prev, best = 0xFFFFFFFFu;
        while (x > 1) {
            uint32_t p = spf[x];
            while (x % p == 0) x /= p;
            uint32_t c = ptr[p];
            while (c <= L && used[c]) c += p;
            ptr[p] = c;
            if (c < best) best = c;
        }
        if (best > L) break;
        used[best] = 1;
        prev = best;
        k++;
        if (best == P) res = k - 1;
    }
    free(spf); free(ptr); free(used); free(primes);
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    return A065204_a(n);
}
