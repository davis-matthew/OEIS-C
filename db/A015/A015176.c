/*
 * A015176  Sum of (Gaussian) q-binomial coefficients for q=-11.
 * https://oeis.org/A015176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{k=0}^n [n choose k]_q where q = -11 (Gaussian
 *            q-binomial coefficients)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 8, i128 n <= 12,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We assume the helper routines seq_alloc_new_array and seq_free_array
   are available (see <../utils/seq_alloc_new_array.h> and
   <../utils/seq_free_array.h> or equivalent). They allocate an array
   of seq_t and free it, respectively. */
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Gauss­ian q‑binomial sum with q = –11.
       We use a dynamic programme that keeps the current row of the
       q‑Pascal triangle.  For each n we build the row from the top
       downwards, updating only O(n) elements per row.  After the last
       row is finished we add up all its entries.  Since the numbers
       grow rapidly the routine is only practical for the ranges
       used in the test suite (up to a few thousand).  It works
       for arbitrary‑precision seq_t types as well.  */

    if (n == 0) {
        return (seq_t)1;
    }

    /* Pre‑compute powers of q = –11 up to exponent n. */
    seq_t *pow_q = seq_alloc_new_array((size_t)n + 1);
    seq_t base_q = (seq_t)(-11);
    pow_q[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        pow_q[i] = pow_q[i - 1] * base_q;
    }

    /* Temporary array holding the current row of q‑Pascal triangle. */
    seq_t *dp = seq_alloc_new_array((size_t)n + 1);
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;          /* [0 choose 0] = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* Update row i from the back.  At step k we need
           q^{i-k} = pow_q[i-k].  We keep a running multiplier
           to avoid an indexing operation. */
        seq_t mul = (seq_t)1;         /* mul = q^{i-k}  initially q^{0} */
        for (idx_t k = i; k >= 1; --k) {
            dp[k] = dp[k] + mul * dp[k - 1];
            mul = mul * base_q;       /* next multiplication by q = –11 */
        }
    }

    /* Sum all binomial coefficients of the last row. */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += dp[k];
    }

    /* Clean up temporary storage. */
    seq_free_array(pow_q);
    seq_free_array(dp);

    return sum;
}
