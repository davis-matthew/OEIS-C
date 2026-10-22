/*
 * A006601  Numbers k such that k, k+1, k+2 and k+3 have the same number of
 *          divisors.
 * https://oeis.org/A006601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   segmented sieve computing d(k) for each k; a(n) = n-th k with
 *            d(k)=d(k+1)=d(k+2)=d(k+3)
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define BLK 65536

static uint32_t *primes = NULL;
static uint64_t nprimes = 0;
static uint64_t plim = 0;

static void make_primes(uint64_t lim) {
    uint64_t i, j;
    char *s = (char *)calloc(lim + 1, 1);
    free(primes);
    primes = (uint32_t *)malloc((lim / 2 + 2) * sizeof(uint32_t));
    nprimes = 0;
    for (i = 2; i <= lim; i++) {
        if (!s[i]) {
            primes[nprimes++] = (uint32_t)i;
            for (j = i * i; j <= lim; j += i) s[j] = 1;
        }
    }
    free(s);
    plim = lim;
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t found = 0;
    static uint64_t next_lo = 1;
    static uint64_t prev_d = 0;
    static uint64_t run = 0;
    static uint64_t rem[BLK];
    static uint32_t dv[BLK];

    if (n < 1) return (seq_t)0;
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (uint64_t *)malloc(cache_cap * sizeof(uint64_t));
    }
    if (plim == 0) make_primes(1 << 16);

    while (found < n) {
        uint64_t lo = next_lo, hi = lo + BLK - 1, j, pi;
        while (plim * plim < hi) make_primes(plim * 2);
        for (j = 0; j < BLK; j++) {
            rem[j] = lo + j;
            dv[j] = 1;
        }
        for (pi = 0; pi < nprimes; pi++) {
            uint64_t p = primes[pi];
            uint64_t st;
            if (p * p > hi) break;
            st = ((lo + p - 1) / p) * p;
            for (j = st - lo; j < BLK; j += p) {
                uint64_t m = rem[j];
                uint32_t e = 0;
                while (m % p == 0) {
                    m /= p;
                    e++;
                }
                rem[j] = m;
                dv[j] *= e + 1;
            }
        }
        for (j = 0; j < BLK; j++) {
            uint64_t d = dv[j];
            if (rem[j] > 1) d *= 2;
            if (d == prev_d) run++;
            else run = 1;
            prev_d = d;
            if (run >= 4) {
                if (found >= cache_cap) {
                    idx_t nc = cache_cap * 2;
                    cache = (uint64_t *)realloc(cache, nc * sizeof(uint64_t));
                    cache_cap = nc;
                }
                cache[found++] = lo + j - 3;
            }
        }
        next_lo = hi + 1;
    }
    return (seq_t)cache[n - 1];
}
