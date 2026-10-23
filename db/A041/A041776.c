/*
 * A041776  Numerators of continued fraction convergents to sqrt(409).
 * https://oeis.org/A041776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A041776 – Numerators of continued fraction convergents to sqrt(409).  
 * Recurrence: let c_0 = 20 and for k≥1,
 *     c_k = 4,2,7,1,1,1,4,2,2,13,13,2,2,4,1,1,1,7,2,4,40 (periodic, length 21);
 * Then a(0)=20, a(1)=81 and for n≥2
 *     a(n) = c_n * a(n-1) + a(n-2).
 */

#include <stdlib.h>

/* Periodic part of the continued fraction (excluding the first term c_0=20). */
static const seq_t period_arr[21] = {
    4, 2, 7, 1, 1, 1, 4, 2, 2, 13, 13, 2, 2, 4, 1, 1, 1, 7, 2, 4, 40
};
static const idx_t period_len = 21;

/* Cache of computed numerators. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms stored in cache */

/* Compute a(n) – numerator of the nth convergent. */
seq_t a(idx_t n)
{
    /* Grow cache if necessary. */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) { /* allocation failed */
            /* In contest environment we can abort; otherwise handle error. */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;

        /* Determine previous two numerators for initialization. */
        seq_t prev2, prev1;
        if (old_len == 0) {
            prev2 = (seq_t)0;          /* p_{-2} */
            prev1 = (seq_t)1;          /* p_{-1} */
        } else if (old_len == 1) {
            prev2 = (seq_t)1;          /* p_{-1} */
            prev1 = cache[0];          /* p_0 */
        } else {
            prev2 = cache[old_len - 2]; /* p_{i-2} */
            prev1 = cache[old_len - 1]; /* p_{i-1} */
        }

        /* Compute missing terms up to n. */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t c = (i == 0) ? (seq_t)20
                               : period_arr[(size_t)((i - 1) % period_len)];
            seq_t p = c * prev1 + prev2;
            cache[i] = p;
            prev2 = prev1;
            prev1 = p;
        }
        cache_len = new_len;
    }

    return cache[n];
}
