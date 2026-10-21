/*
 * A058583  Max_{k=0..n} k!*|Stirling1(n,k)|.
 * https://oeis.org/A058583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{k=0..n} k!*|Stirling1(n,k)| Recurrence for v_k(n) =
 *            k!*|Stirling1(n,k)|: v_k(n) = k*v_{k-1}(n-1) + (n-1)*v_k(n-1)
 *            with v_0(0)=1, v_k(0)=0 for k>0. The maximum of v_k(n) over k is
 *            a(n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of a(k) values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;            /* number of computed a(k) values */

    /* static array holding the previous row v_k(n-1) */
    static seq_t *prev = NULL;
    static size_t prev_len = 0;             /* size of prev = (n-1)+1 */

    /* initialise at n = 0 */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;                /* a(0) = 1 */
        cache_len = 1;

        prev = (seq_t *)malloc(sizeof(seq_t));
        prev[0] = (seq_t)1;                /* v_0(0) = 1 */
        prev_len = 1;
    }

    /* compute rows up to requested n */
    while (cache_len <= n) {
        size_t current_n = cache_len - 1;   /* previous n (prev row)   */
        size_t next_n    = current_n + 1;   /* n to compute           */
        size_t new_len   = next_n + 1;      /* new row size (k = 0..next_n) */

        seq_t *curr = (seq_t *)malloc(new_len * sizeof(seq_t));

        seq_t max_val = (seq_t)0;           /* maximum of current row */

        for (idx_t k = 0; k < (idx_t)new_len; ++k) {
            seq_t term1 = 0;
            seq_t term2 = 0;

            if (k > 0) {
                term1 = prev[k - 1] * (seq_t)k;           /* k * prev[k-1]   */
            }
            if ((idx_t)current_n >= k) {
                term2 = prev[k] * (seq_t)current_n;       /* (n-1) * prev[k] */
            }

            seq_t val = term1 + term2;
            curr[k] = val;

            if (val > max_val) {
                max_val = val;
            }
        }

        /* store a(next_n) */
        seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – abort quietly */
            free(curr);
            return (seq_t)0;
        }
        cache = tmp;
        cache[cache_len] = max_val;
        cache_len++;

        /* move to next row */
        free(prev);
        prev = curr;
        prev_len = new_len;
    }

    return cache[n];
}
