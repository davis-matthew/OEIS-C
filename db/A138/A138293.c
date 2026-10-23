/*
 * A138293  E.g.f. satisfies: A(x) = x + exp(x*A(x)).
 * https://oeis.org/A138293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A138293 (OEIS)
   • Functional equation:  A(x) = x + exp(x * A(x))
   • Recurrence (derived from the compositional functional equation):
        e_k   = k * a(k-1)                       (k ≥ 1)
        b_0   = 1
        b_n   = Σ_{k=1..n} C(n-1,k-1) * e_k * b_{n-k}   (n ≥ 1)
        a_0   = 1
        a_1   = 1 + b_1
        a_n   = b_n                            (n ≥ 2)
   The function below implements this recurrence, computing the
   sequence on demand and memoising results for subsequent calls.
*/

#include <stdlib.h>

/* a(n) when 0 ≤ n ≤ 10000 (or larger).  The function is safe for
   arbitrary precision types seq_t, but keeps intermediate values
   as small as the recurrence allows.  It uses only integer
   arithmetic, no floating point, and no external utility headers.  */
seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *a_arr = NULL;
    static seq_t *b_arr = NULL;
    static seq_t *e_arr = NULL;
    static idx_t cache_size = 0;

    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = (n + 1);
        a_arr = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
        b_arr = (seq_t *)realloc(b_arr, new_size * sizeof(seq_t));
        e_arr = (seq_t *)realloc(e_arr, new_size * sizeof(seq_t));

        /* Initialise new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                a_arr[i] = (seq_t)1;     /* a_0 */
                b_arr[i] = (seq_t)1;     /* b_0 */
                /* e_arr[0] unused */
            } else {
                /* e_i will be computed later; set placeholder 0 */
                e_arr[i] = (seq_t)0;
            }
        }
        cache_size = new_size;
    }

    /* Compute new values sequentially up to n */
    for (idx_t cur = 1; cur <= n; ++cur) {
        if (cur == 1) {
            /* e_1 = 1 * a_0 */
            e_arr[1] = (seq_t)1 * a_arr[0];
        } else {
            /* e_cur = cur * a_{cur-1} */
            e_arr[cur] = (seq_t)cur * a_arr[cur - 1];
        }

        /* Compute b_cur = Σ_{k=1..cur} C(cur-1,k-1) * e_k * b_{cur-k} */
        seq_t sum = 0;
        seq_t bin = 1; /* C(cur-1, 0) = 1 */
        for (idx_t k = 1; k <= cur; ++k) {
            /* term = bin * e_k * b_{cur-k} */
            seq_t term = bin * e_arr[k];
            term = term * b_arr[cur - k];
            sum += term;

            /* Update binomial coefficient for next k */
            if (k < cur) {
                /* bin = bin * (cur-1 - (k-1)) / k,
                   i.e. bin = bin * (cur - k) / k */
                seq_t num = (seq_t)(cur - k);
                seq_t den = (seq_t)k;
                bin = (bin * num) / den;
            }
        }
        b_arr[cur] = sum;

        /* Set a_cur according to the recurrence */
        if (cur == 1) {
            a_arr[cur] = (seq_t)1 + b_arr[cur]; /* a_1 = 1 + b_1 */
        } else {
            a_arr[cur] = b_arr[cur];            /* a_n = b_n for n≥2 */
        }
    }

    return a_arr[n];
}
