/*
 * A185895  Exponential generating function is
 *          (1-x^1/1!)(1-x^2/2!)(1-x^3/3!)....
 * https://oeis.org/A185895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of x^n in the exponential generating
 *            function Π_{k≥1}(1 - x^k / k!), multiplied by n!. It can be
 *            computed by iteratively multiplying the polynomial by each
 *            factor. The following implementation uses an integer recurrence
 *            derived from this product: Start with a[0] = 1, a[i]...
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 18, i64 n <= 29, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t binom_u64(idx_t n, idx_t k)
{
    /* Compute binomial coefficient C(n, k) as a seq_t.  k may be bigger
       than n/2; reduce it to the smaller value. */
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i; using integer arithmetic */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Allocate array for the coefficient sequence up to n. */
    seq_t *a = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a) return (seq_t)0;          /* fallback: return 0 on allocation failure */

    for (idx_t i = 0; i <= n; ++i) a[i] = 0;
    a[0] = 1;                         /* base case: a(0) = 1 */

    /* Main recurrence: iterate over each factor (1 - x^k/k!). */
    for (idx_t k = 1; k <= n; ++k) {
        /* B holds binom(m, k) for the current value of m,
           starting with m = n. */
        seq_t B = binom_u64(n, k);
        for (idx_t m = n; m >= k; --m) {
            /* a[m] -= binom(m, k) * a[m - k] */
            a[m] -= B * a[m - k];

            /* Update B to binom(m-1, k) for the next iteration
               (if we are not at the last step). */
            if (m > k) {
                B = B * (seq_t)(m - k) / (seq_t)m;
            }
        }
    }

    seq_t result = a[n];
    free(a);
    return result;
}
