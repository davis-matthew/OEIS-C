/*
 * A063897  a(n) is the least k such that k - A000215(j), j=0..n, are all
 *          primes.
 * https://oeis.org/A063897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   search over primes p (segmented sieve, residue filter,
 *            Miller-Rabin): a(n) = F(n) + p for the least prime p such that p
 *            + F(n) - F(j) is prime for all j = 0..n-1, where F(j) =
 *            A000215(j) = 2^(2^j) + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-07)
 * Helpers:   mem, mulmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mulmod_seq.h"
#include "../utils/mem.h"

static seq_t A063897_powmod(seq_t b, seq_t e, seq_t m)
{
    seq_t r = 1;
    b = b % m;
    while (e > 0) {
        if ((e & 1) != 0) r = mulmod_seq(r, b, m);
        e >>= 1;
        if (e > 0) b = mulmod_seq(b, b, m);
    }
    return r;
}

/* Miller-Rabin with bases 2..41 (deterministic below 3.3e24) */
static bool A063897_isprime(seq_t m)
{
    static const unsigned bases[13] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
    seq_t d, x, m1;
    unsigned s = 0, r;
    int i;
    if (m < 2) return false;
    for (i = 0; i < 13; i++) {
        if (m == bases[i]) return true;
        if (m % bases[i] == 0) return false;
    }
    if (m < 43 * 43) return true;
    m1 = m - 1;
    d = m1;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 13; i++) {
        x = A063897_powmod((seq_t)bases[i], d, m);
        if (x == 1 || x == m1) continue;
        for (r = 1; r < s; r++) {
            x = mulmod_seq(x, x, m);
            if (x == m1) break;
        }
        if (r >= s) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t *F = mem_new(seq_t, (size_t)n + 1);
    seq_t *D = mem_new(seq_t, (size_t)n + 1);
    idx_t i, j;
    const uint32_t BL = 65536;
    unsigned char *cb;
    uint32_t *bp;
    uint32_t *bad;
    size_t nb = 0, k;
    uint32_t u, v;
    const size_t S = (size_t)1 << 20;
    unsigned char *comp;
    uint64_t lo = 0;
    seq_t res = 0;

    F[0] = 3;
    for (i = 0; i < n; i++) {
        seq_t t = F[i] - 1;
        F[i + 1] = t * t + 1;
    }
    for (j = 0; j < n; j++) D[j] = F[n] - F[j];

    cb = (unsigned char *)malloc((size_t)BL + 1);
    memset(cb, 0, (size_t)BL + 1);
    for (u = 2; u * u <= BL; u++)
        if (!cb[u])
            for (v = u * u; v <= BL; v += u) cb[v] = 1;
    bp = (uint32_t *)malloc(sizeof(uint32_t) * 7000);
    for (u = 2; u <= BL; u++)
        if (!cb[u]) bp[nb++] = u;
    free(cb);

    /* bad[k*n + j]: residue of p mod bp[k] making p + D[j] divisible by bp[k] */
    bad = (uint32_t *)malloc(sizeof(uint32_t) * (nb * (size_t)n + 1));
    for (k = 0; k < nb; k++) {
        uint32_t q = bp[k];
        for (j = 0; j < n; j++) {
            uint32_t rr = (uint32_t)(long long)(D[j] % (seq_t)q);
            bad[k * (size_t)n + (size_t)j] = (q - rr) % q;
        }
    }

    comp = (unsigned char *)malloc(S);
    for (;;) {
        uint64_t hi = lo + (uint64_t)S; /* exclusive */
        size_t x;
        memset(comp, 0, S);
        if (lo == 0) { comp[0] = 1; comp[1] = 1; }
        for (k = 0; k < nb; k++) {
            uint64_t b = bp[k];
            uint64_t st;
            if (b * b >= hi) break;
            st = b * b;
            if (st < lo) st = ((lo + b - 1) / b) * b;
            for (x = (size_t)(st - lo); x < S; x += (size_t)b) comp[x] = 1;
        }
        for (x = 0; x < S; x++) {
            uint64_t p;
            bool ok = true;
            if (comp[x]) continue;
            p = lo + (uint64_t)x;
            /* p + D[j] > p >= q, so divisibility by q means composite */
            for (k = 0; k < nb && ok && (uint64_t)bp[k] < p; k++) {
                uint32_t pm = (uint32_t)(p % bp[k]);
                const uint32_t *bk = bad + k * (size_t)n;
                for (j = 0; j < n; j++) {
                    if (pm == bk[j]) { ok = false; break; }
                }
            }
            if (!ok) continue;
            for (j = 0; j < n; j++) {
                if (!A063897_isprime((seq_t)p + D[j])) { ok = false; break; }
            }
            if (ok) {
                res = (seq_t)p + F[n];
                free(comp);
                free(bad);
                free(bp);
                mem_free(D);
                mem_free(F);
                return res;
            }
        }
        lo = hi;
    }
}
