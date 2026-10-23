/*
 * A026833  Number of partitions of n into distinct parts, the least being
 *          even.
 * https://oeis.org/A026833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts whose
 *            smallest part is even. Computed by dynamic programming over the
 *            generating function product_{j>=t} (1+x^j)
 * Range:     u8 n <= 36, u16 n <= 90, u32 n <= 257, u64 n <= 826,
 *            u128 n <= 2887, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Transition: for every t = n+1 … 1,
 *   cur[s] = number of partitions of s into distinct parts >= t.
 * The product for t is obtained by multiplying the previous product
 *  with the factor (1+x^t).  When t is odd (t=2k+1) we add the
 *  coefficient of x^{n-2k} to the answer.  The loop is started at
 *  n+1 so that the cases with n<3 are handled correctly. */

seq_t a(idx_t n)
{
    if (n < 2)               /* a(0)=a(1)=0 */
        return (seq_t)0;

    /* Allocate array for the DP; size n+1 to hold sums 0..n. */
    seq_t *cur = alloc_seq_array(n + 1);
    /* Initialise to zero. */
    for (idx_t i = 0; i <= n; ++i) cur[i] = (seq_t)0;
    cur[0] = (seq_t)1;

    seq_t ans = (seq_t)0;

    /* t runs from n+1 down to 1; for t>n the inner update does nothing. */
    for (idx_t t = n + 1; t > 0; --t) {
        if (t <= n) {
            for (idx_t s = n; s >= t; --s) {
                cur[s] += cur[s - t];
            }
        }
        if ((t & (idx_t)1) && t >= 3) {
            idx_t k = (t - 1) / 2;
            idx_t target = n - 2 * k;
            if (target <= n)
                ans += cur[target];
        }
    }

    free_seq_array(cur);
    return ans;
}
