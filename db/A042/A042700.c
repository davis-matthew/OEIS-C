/*
 * A042700  Numerators of continued fraction convergents to sqrt(880).
 * https://oeis.org/A042700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence A042700: Numerators of continued fraction convergents to sqrt(880)
 * Generation method: use the recurrence  
 *   p_{-2} = 0, p_{-1} = 1
 *   p_k   = a_k * p_{k-1} + p_{k-2}
 * where a_k are the partial quotients of sqrt(880):
 *   a_0 = 29, and for k >= 1 the period is [1, 1, 1, 58] repeated.
 * The function returns a(n) = p_n for n >= 0, caching results for
 * efficient repeated calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size */
    static idx_t computed = 0;     /* first index not yet computed */

    /* cached values for last two numerators */
    static seq_t p_prev2 = 0;      /* p_{k-2} */
    static seq_t p_prev1 = 1;      /* p_{k-1} */

    /* ensure cache can hold up to n */
    if (cache_size <= n) {
        size_t new_size = cache_size ? cache_size * 2 : 16;
        if (new_size <= (size_t)n + 1) new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;    /* allocation failure: return 0 (not expected) */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute terms up to n */
    for (idx_t k = computed; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = 29;
        } else {
            /* period [1,1,1,58] starting at k=1 */
            switch ((k - 1) % 4) {
                case 0: a_k = 1; break;
                case 1: a_k = 1; break;
                case 2: a_k = 1; break;
                case 3: a_k = 58; break;
            }
        }
        seq_t new_val = p_prev1 * a_k + p_prev2;
        cache[k] = new_val;
        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = new_val;
    }

    computed = n + 1;  /* next index to compute if needed */
    return cache[n];
}
