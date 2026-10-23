/*
 * A191391  Number of horizontal segments in all dispersed Dyck paths of
 *          length n (i.e., in all Motzkin paths of length n with no
 *          (1,0)-steps at positive heights; a horizontal segment is a maximal
 *          sequence of consecutive (1,0)-steps).
 * https://oeis.org/A191391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - C(n-1, floor(n/2)-1) for n ≥ 1, a(0) = 0
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* power of two part */
    seq_t pow = pow2(n - 1);

    /* binomial subtraction part */
    idx_t half = n / 2;
    seq_t bin = 0;
    if (half > 0)
        bin = binomial(n - 1, half - 1);

    return pow - bin;
}
