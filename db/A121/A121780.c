/*
 * A121780  Series expansion for mean-squared radius of gyration of stack
 *          polygons on square lattice.
 * https://oeis.org/A121780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 35, u128 n <= 55,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = p_n - Σ_{k=1}^{10} q_k * a(n-k),
 * where   q = [1, -15,  95, -330,  685, -873, 685, -330,  95, -15, 1]
 * and      p = [0, 0, 8, -54, 214, -489, 605, -386, 177, -120, 19, -1]
 * (p_n = 0 for n > 11).  The initial values are a(1)=0, a(2)=8, a(3)=66, ...
 * This recurrence follows from the generating function
 * x^2*(8-54x+214x^2-489x^3+605x^4-386x^5+177x^6-120x^7+19x^8-x^9)/(1-3x+x^2)^5.
 *
 * The implementation stores values in a static cache so that calls may
 * come in any order.  It works for both compile modes provided by the
 * OEIS helper library (plain integers or arbitrary‑precision integers).
 */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (outside the official offset). */
    if (n == 0) return 0;

    /* Coefficients of the denominator polynomial Q(x) = (1-3x+x^2)^5
       and of the numerator P(x) = x^2*(8-54x+...-x^9). */
    static const int q_raw[11] = {
        1,   /* Q[0]  = 1  */
        -15, /* Q[1]  =-15 */
        95,  /* Q[2]  = 95 */
        -330,/* Q[3]  =-330*/
        685, /* Q[4]  = 685*/
        -873,/* Q[5]  =-873*/
        685, /* Q[6]  = 685*/
        -330,/* Q[7]  =-330*/
        95,  /* Q[8]  = 95 */
        -15, /* Q[9]  =-15 */
        1    /* Q[10] = 1  */
    };
    static const int p_raw[12] = {
        0,   /* P[0] */
        0,   /* P[1] */
        8,   /* P[2] */
        -54, /* P[3] */
        214, /* P[4] */
        -489,/* P[5] */
        605, /* P[6] */
        -386,/* P[7] */
        177, /* P[8] */
        -120,/* P[9] */
        19,  /* P[10]*/
        -1   /* P[11]*/
    };

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* Number of cached terms (indices 0..len-1) */
    static size_t cache_cap = 0;  /* Allocated capacity of cache array */

    /* Expand cache if necessary */
    if (n >= cache_len) {
        size_t new_cap = (size_t)(n) + 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) return 0;                /* allocation failure: return 0 */
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
            } else if (i == 1) {
                cache[1] = 0;
            } else {
                seq_t sum = 0;
                /* Σ_{k=1}^{10} q_k * a(i-k)   (only if i>=k) */
                for (int k = 1; k <= 10; ++k) {
                    if (i < (idx_t)k) break;
                    seq_t term = cache[i - k] * (seq_t)q_raw[k];
                    sum += term;
                }
                seq_t p_term = 0;
                if (i < 12) p_term = (seq_t)p_raw[i];
                cache[i] = p_term - sum;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
