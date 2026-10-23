/*
 * A144126  P_6(2n+1), the Legendre polynomial of order 6 at 2n+1.
 * https://oeis.org/A144126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 924*n^6 + 2772*n^5 + 3150*n^4 + 1680*n^3 + 420*n^2 + 42*n
 *            + 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 12, u64 n <= 520, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Cast the index to seq_t once to avoid repeated conversions
    seq_t nn = (seq_t)n;

    // Horner's method for efficient polynomial evaluation:
    // a(n) = (((((924*n + 2772)*n + 3150)*n + 1680)*n + 420)*n + 42)*n + 1
    seq_t result = 924 * nn + 2772;
    result = result * nn + 3150;
    result = result * nn + 1680;
    result = result * nn + 420;
    result = result * nn + 42;
    result = result * nn + 1;

    return result;
}
