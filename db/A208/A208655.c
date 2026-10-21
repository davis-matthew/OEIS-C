/*
 * A208655  Number of palindromic paths starting with 1 through the subset
 *          array of {1,2,...,n}; see Comments.
 * https://oeis.org/A208655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=2}^{⌊n/2⌋} ( n * C(n-1, k-1)^2 ) * ( if n odd then n
 *            * C(n-1, (n-1)/2) ) where C(a,b) is the binomial coefficient
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = 1;
    idx_t half = n / 2;                     /* floor(n/2)                     */
    idx_t max_k = (n - 1) / 2;               /* max value of k in binomial calc */
    seq_t binom = 1;                         /* C(n-1,0) = 1                   */
    seq_t mid_m = 0;                         /* holds C(n-1,(n-1)/2) if needed  */

    for (idx_t k = 1; k <= max_k; ++k) {
        /* compute C(n-1, k) incrementally:
           C(n-1,k) = C(n-1,k-1) * (n-1-(k-1)) / k           */
        seq_t num = (seq_t)(n - 1 - (k - 1));
        seq_t den = (seq_t)k;
        binom = binom * num / den;           /* exact division                 */

        idx_t r = k + 1;                     /* position used in palindrome    */
        if (r <= half) {
            seq_t m = binom;
            result *= (seq_t)n;
            result *= m;
            result *= m;
        }
        /* store middle binomial if n is odd and r is the middle position */
        if ((n & 1) && (r == half + 1)) {
            mid_m = binom;
        }
    }

    if (n & 1) { /* odd n: multiply by n * middle binomial */
        result *= (seq_t)n;
        result *= mid_m;
    }

    return result;
}
