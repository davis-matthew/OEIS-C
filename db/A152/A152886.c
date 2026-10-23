/*
 * A152886  Number of descents beginning and ending with an even number in all
 *          permutations of {1,2,...,n}.
 * https://oeis.org/A152886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * binomial(floor(n/2), 2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* floor(n/2) is the number of even elements in {1,…,n} */
    idx_t k = n / 2;

    /* For k < 2 the binomial coefficient is zero → a(n) = 0 */
    if (k < 2) {
        return 0;
    }

    /* binomial(k,2) = k*(k-1)/2  (handled by the helper) */
    seq_t bin = binomial(k, 2);

    /* (n-1)! – use the utility that handles arbitrary‐precision */
    seq_t fact = factorial(n - 1);

    /* a(n) = (n-1)! * binomial(k,2) */
    return fact * bin;
}
