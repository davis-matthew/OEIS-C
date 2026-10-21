/*
 * A236682  Values of a for triples (a,b,c) of positive integers such that 1/a
 *          + 1/b + 1/c = 1/2 and a <= b <= c, listed with multiplicity.
 * https://oeis.org/A236682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by enumerating all triples (a,b,c) of positive integers satisfying
   1/a + 1/b + 1/c = 1/2 with a <= b <= c, and ordering triples lexicographically.
   Return the first component a of the nth triple; return 0 if n is larger
   than the number of solutions. */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache == NULL) {
        const idx_t a_min = 3;
        const idx_t a_max = 6;

        cache_cap = 16; /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;

        for (idx_t a = a_min; a <= a_max; ++a) {
            /* Upper bound for b: solve 1/a + 2/b >= 1/2  ->  b <= 4a/(a-2) */
            idx_t bmax = (4 * a) / (a - 2);
            for (idx_t b = a; b <= bmax; ++b) {
                idx_t D = (a - 2) * b - 2 * a;
                if (D <= 0) continue;
                idx_t numer = 2 * a * b;
                if (numer % D != 0) continue;
                idx_t c = numer / D;
                if (c < b) continue; /* ensure a <= b <= c */

                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                }
                cache[cache_len++] = (seq_t)a;
            }
        }
    }

    if (n == 0 || n > cache_len) return (seq_t)0;
    return cache[n - 1];
}
