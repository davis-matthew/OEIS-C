/*
 * A192107  Sum of all the n-digit numbers whose digits are all odd.
 * https://oeis.org/A192107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 55*a(n-1) - 250*a(n-2), a(1)=25, a(2)=1375
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* dynamic cache of computed values */
static idx_t a_cache_size = 0;  /* allocated capacity of the cache      */
static idx_t a_last = 0;        /* highest index that has been computed */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* undefined for n==0 */

    /* Ensure the cache holds indices up to n */
    idx_t needed = n + 1;                       /* indices 0..n inclusive */
    if (a_cache_size < needed) {
        size_t new_size = a_cache_size == 0 ? 3 : a_cache_size;
        while (new_size < needed) new_size <<= 1;   /* grow to power of two */

        seq_t *new_arr = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (a_cache) {
            /* copy only the part that has been computed */
            for (idx_t i = 0; i <= a_last; ++i) {
                new_arr[i] = a_cache[i];
            }
            free(a_cache);
        }
        a_cache = new_arr;
        a_cache_size = new_size;
    }

    /* Initial values */
    if (a_last == 0) {
        a_cache[1] = (seq_t)25;
        a_cache[2] = (seq_t)1375;
        a_last = 2;
    }

    /* Compute missing values up to n using the recurrence */
    if (n > a_last) {
        for (idx_t i = a_last + 1; i <= n; ++i) {
            a_cache[i] = a_cache[i - 1] * (seq_t)55 - a_cache[i - 2] * (seq_t)250;
        }
        a_last = n;
    }

    return a_cache[n];
}
