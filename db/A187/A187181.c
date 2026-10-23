/*
 * A187181  Parse the infinite string 012012012012... into distinct phrases 0,
 *          1, 2, 01, 20, 12, 012, ...; a(n) = length of n-th phrase.
 * https://oeis.org/A187181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10), for n>12 Initial values:
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=2, a(6)=2, a(7)=3, a(8)=4,
 *            a(9)=3, a(10)=4, a(11)=3, a(12)=4
 * Range:     u8 n <= 763, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated size (including index 0) */
    static idx_t computed_max = 0;        /* largest index computed so far */

    if (n < 1)    /* undefined for n < 1; return 0 as a safe default */
        return (seq_t)0;

    /* Ensure the cache can hold index n (1‑based).  We keep index 0 unused. */
    size_t target = (size_t)n + 1;        /* total entries needed: 0..n */
    if (cache_cap < target) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – terminate the program. */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms only when needed. */
    if (n > computed_max) {
        static const seq_t init[12] = {
            (seq_t)1,(seq_t)1,(seq_t)1,
            (seq_t)2,(seq_t)2,(seq_t)2,
            (seq_t)3,(seq_t)4,(seq_t)3,
            (seq_t)4,(seq_t)3,(seq_t)4
        };
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i <= 12) {
                cache[(size_t)i] = init[i - 1];
            } else {
                cache[(size_t)i] = cache[(size_t)(i - 1)] + cache[(size_t)(i - 9)]
                                     - cache[(size_t)(i - 10)];
            }
        }
        computed_max = n;
    }

    return cache[(size_t)n];
}
