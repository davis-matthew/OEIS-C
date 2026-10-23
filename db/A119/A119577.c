/*
 * A119577  a(n) = (n+n^2+n^3)*(binomial(2*n,n))/2.
 * https://oeis.org/A119577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n + n^2 + n^3) * binomial(2*n, n) / 2
 * This implementation uses the provided binomial function which
 * supports arbitrary precision for the seq_t type.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    // Convert the index to seq_t once.
    seq_t ns = (seq_t)n;

    // Compute the polynomial part: n + n^2 + n^3.
    seq_t poly = ns + ns * ns + ns * ns * ns;

    // Compute the central binomial coefficient C(2n, n).
    seq_t bin = binomial(2 * n, n);

    // Combine them and divide by 2.  The division is exact.
    seq_t result = (poly * bin) / (seq_t)2;

    return result;
}
