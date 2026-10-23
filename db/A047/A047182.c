/*
 * A047182  Number of nonempty subsets of {1,2,...,n} in which exactly 1/2 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n, floor((n-2)/2)) - 1
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 1, 2, 3 the formula yields 0 */
    if (n < 4)
        return 0;

    /* Compute k = floor((n-2)/2) safely */
    idx_t k = (n - 2) / 2;

    /* Compute binomial coefficient C(n,k) multiplicatively.
       This works for arbitrary precision seq_t and is correct
       for values that fit within the type. */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }

    /* Subtract the empty subset */
    res -= 1;
    return res;
}
