/*
 * A029826  Expansion of 1/(x^10+x^9-x^7-x^6-x^5-x^4-x^3+x+1) (inverse of
 *          Salem polynomial).
 * https://oeis.org/A029826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 40, i16 n <= 74, i32 n <= 143, i64 n <= 279,
 *            i128 n <= 552, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A029826 – Expansion of 
 *  1/(x^10 + x^9 - x^7 - x^6 - x^5 - x^4 - x^3 + x + 1)
 *
 *  Recurrence:
 *    a(n) = -a(n-1) + a(n-3) + a(n-4) + a(n-5) + a(n-6) + a(n-7)
 *           - a(n-9) - a(n-10)
 *
 *  Initial values (n = 0 … 9):
 *    a(0) =  1
 *    a(1) = -1
 *    a(2) =  1
 *    a(3) =  0
 *    a(4) =  0
 *    a(5) =  1
 *    a(6) =  0
 *    a(7) =  1
 *    a(8) =  0
 *    a(9) =  1
 *
 *  The function memoises computed values and works in O(n) time 
 *  with any seq_t type. */

#include <stddef.h>
#include "../utils/seq_realloc.h"   /* realloc for seq_t arrays */

static seq_t *seq_cache = NULL;       /* allocated array */
static size_t cache_len = 0;          /* current capacity of seq_cache */
static size_t computed_len = 0;       /* first index not yet computed */

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ensure the array can hold index n */
    size_t need = (size_t)n + 1;
    if (need < 10) need = 10;          /* at least the 10 initial terms */
    if (cache_len < need) {
        seq_cache = seq_realloc(seq_cache, cache_len, need);
        cache_len = need;
    }

    /* initialise the first 10 terms once */
    if (computed_len == 0) {
        const seq_t base[10] = {
            (seq_t) 1, (seq_t)-1, (seq_t)1, (seq_t)0, (seq_t)0,
            (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)1
        };
        for (size_t i = 0; i < 10; ++i)
            seq_cache[i] = base[i];
        computed_len = 10;
    }

    /* compute new terms if necessary */
    if (computed_len < need) {
        for (size_t i = computed_len; i < need; ++i) {
            seq_cache[i] =
                - seq_cache[i-1]
                + seq_cache[i-3] + seq_cache[i-4] + seq_cache[i-5]
                + seq_cache[i-6] + seq_cache[i-7]
                - seq_cache[i-9] - seq_cache[i-10];
        }
        computed_len = need;
    }

    return seq_cache[n];
}
