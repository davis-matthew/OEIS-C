/*
 * A168222  a(n) = A006369(A006369(n)).
 * https://oeis.org/A168222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A168222
   a(n) = 2*a(n-9) - a(n-18)   for n > 17
   a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 7, a(5) = 9, a(6) = 5,
   a(7) = 6, a(8) = 15, a(9) = 4, a(10)=17, a(11)=10, a(12)=11,
   a(13)=23, a(14)=25, a(15)=13, a(16)=14, a(17)=31 */

#include <stdlib.h>   /* malloc, realloc */

/* initial 18 terms */
static const seq_t init_vals[18] = {
    0, 1, 2, 3, 7, 9, 5, 6, 15, 4,
    17, 10, 11, 23, 25, 13, 14, 31
};

/* dynamic cache of computed values */
static seq_t *cache     = NULL;
static size_t   cap     = 0;    /* allocated size */
static size_t   cur_len = 0;    /* next free index */

/* ensure that the cache can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cap <= (size_t)n) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* first call: initialise cache with the 18 known terms */
    if (cache == NULL) {
        ensure_capacity(17);
        for (idx_t i = 0; i < 18; ++i)
            cache[(size_t)i] = init_vals[(size_t)i];
        cur_len = 18;
    }

    /* extend cache up to n if necessary */
    if ((size_t)n >= cur_len) {
        ensure_capacity(n);
        for (idx_t i = (idx_t)cur_len; i <= n; ++i) {
            /* recurrence applies for i >= 18, but cur_len starts at 18 */
            cache[(size_t)i] = (seq_t)2 * cache[(size_t)(i - 9)]
                               - cache[(size_t)(i - 18)];
        }
        cur_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
