/*
 * A184388  a(n) = product of numbers from 1 to sigma(n), where sigma(n) =
 *          A000203(n).
 * https://oeis.org/A184388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n)! = A000142(A000203(n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   factorial, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the product of the integers 1 through the sum of divisors of n. */

#include "../utils/sigma_of.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* safe guard – offset is 1 */

    /* sigma_of returns a seq_t, but it always fits in idx_t for the n ranges tested. */
    idx_t sigma = (idx_t)sigma_of(n);

    return factorial(sigma);
}
