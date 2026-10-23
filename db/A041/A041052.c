/*
 * A041052  Numerators of continued fraction convergents to sqrt(32).
 * https://oeis.org/A041052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let p_{-2} = 0, p_{-1} = 1, a_0 = 5, and for n ≥ 1,
   a_n = 10 if (n-1) mod 4 == 3, otherwise a_n = 1.
   Then the numerator a(n) is p_n where
   p_n = a_n * p_{n-1} + p_{n-2}.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;           /* number of values stored */

    if (cache_len <= n) {
        /* Need to compute up to n.  Expand cache if necessary. */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)    /* out of memory – return 0 as a fail‑safe */
            return (seq_t)0;
        cache = tmp;
        seq_t p_nm2;   /* p_{i-2} */
        seq_t p_nm1;   /* p_{i-1} */
        size_t i_start;

        if (cache_len == 0) {
            /* No values yet.  Start from the very beginning. */
            p_nm2 = 0;
            p_nm1 = 1;
            i_start = 0;
        } else if (cache_len == 1) {
            /* Only a(0) stored.  For the next term we need p_{-1}=1. */
            p_nm2 = 1;
            p_nm1 = cache[0];
            i_start = 1;
        } else {
            /* Resume after the last cached value. */
            p_nm2 = cache[cache_len - 2];
            p_nm1 = cache[cache_len - 1];
            i_start = cache_len;
        }

        for (size_t i = i_start; i <= n; ++i) {
            seq_t a_i;
            if (i == 0) {
                a_i = 5;                     /* a_0 */
            } else {
                idx_t period = ((idx_t)i - 1) % 4;
                a_i = (period == 3) ? (seq_t)10 : (seq_t)1;
            }
            seq_t p_i = a_i * p_nm1 + p_nm2;
            cache[i] = p_i;
            p_nm2 = p_nm1;
            p_nm1 = p_i;
        }
        cache_len = new_len;
    }
    return cache[n];
}
