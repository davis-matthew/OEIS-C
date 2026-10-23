/*
 * A035101  E.g.f. x*(c(x/2)-1)/(1-2*x), where c(x) = g.f. for Catalan numbers
 *          A000108.
 * https://oeis.org/A035101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A035101
 *  Recurrence: a(1)=0, a(2)=1
 *                a(n) = -2*(n-1)*(2*n-3)*a(n-2)+(4*n-3)*a(n-1)   for n>=3
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;           /* cache of computed terms (indices 1..len) */
static idx_t   a_cache_len = 0;         /* number of terms stored in the cache   */
static idx_t   a_cache_cap = 0;         /* allocated capacity for a_cache        */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* sequence is defined for n >= 1 */

    /* grow cache if needed */
    if (a_cache_len < n) {
        /* ensure we have enough space */
        if (a_cache_cap < n) {
            size_t new_cap = a_cache_cap ? a_cache_cap << 1 : (size_t)8;
            while (new_cap < n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failed – return 0 */
            a_cache = tmp;
            a_cache_cap = (idx_t)new_cap;
        }

        /* compute missing terms */
        for (idx_t i = a_cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)0;
            } else if (i == 2) {
                val = (seq_t)1;
            } else {
                /* recurrence: a(i) = -2*(i-1)*(2*i-3)*a(i-2) + (4*i-3)*a(i-1) */
                seq_t coeff1 = (seq_t)2 * (seq_t)(i - 1) * (seq_t)(2 * i - 3);
                seq_t coeff2 = (seq_t)(4 * i - 3);

                /* a_cache indices: a_cache[0] = a(1), so a_cache[i-3] = a(i-2) */
                seq_t term1 = a_cache[i - 3] * coeff1;   /* -2*(i-1)*(2*i-3)*a(i-2) */
                seq_t term2 = a_cache[i - 2] * coeff2;   /* (4*i-3)*a(i-1) */
                val = term2 - term1;
            }
            a_cache[i - 1] = val;
        }
        a_cache_len = n;
    }

    return a_cache[n - 1];
}
