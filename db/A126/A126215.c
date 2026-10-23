/*
 * A126215  a(1)=1. a(n) = sum of the earlier terms, a(k) (for 1<=k<=n-1),
 *          where every integer coprime to a(k) and <= a(k) is also coprime to
 *          n.
 * https://oeis.org/A126215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) = sum_{k=1}^{n-1} a(k) over all k such
 *            that every prime divisor p of n with p <= a(k) also divides a(k)
 * Range:     u8 n <= 17, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index computed so far */
    static size_t cache_cap = 0; /* current allocation size   */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory */
        cache[0] = (seq_t)0;             /* dummy, unused */
        cache_len = 0;
    }

    /* extend cache to contain all indices up to n */
    if (n > cache_len) {
        size_t needed = (size_t)n + 1;     /* need index n */
        if (needed > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;     /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute terms from cache_len+1 up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
                continue;
            }

            /* factorise i into distinct primes (small numbers, trial division) */
            uint64_t facs[16];
            int fc = 0;
            uint64_t m = (uint64_t)i;
            for (uint64_t p = 2; p * p <= m; ++p) {
                if (m % p == 0) {
                    facs[fc++] = p;
                    while (m % p == 0) m /= p;
                }
            }
            if (m > 1 && fc < 16) facs[fc++] = m;

            /* sum qualifying earlier terms */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                seq_t ak = cache[k];
                bool good = true;
                for (int j = 0; j < fc && good; ++j) {
                    uint64_t p = facs[j];
                    seq_t sp = (seq_t)p;
                    if (ak >= sp && (ak % sp) != 0) {
                        good = false;
                    }
                }
                if (good) sum += ak;
            }
            cache[i] = sum;
        }
        cache_len = n;
    }

    return cache[(size_t)n];
}
