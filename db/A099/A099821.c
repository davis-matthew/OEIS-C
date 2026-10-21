/*
 * A099821  Odd positive integers in base 2 (bisection of A007088).
 * https://oeis.org/A099821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007088(2*n-1) = decimal representation of binary of
 *            (2*n-1)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 512, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the index of the desired binary word in A007088. */
    idx_t m = (n << 1) - 1;          /* 2*n - 1, n >= 1 */

    /* Count the number of binary bits needed for m. */
    idx_t bits = 0;
    idx_t temp = m;
    while (temp > 0) {               /* temp will never be 0 for n >= 1 */
        ++bits;
        temp >>= 1;
    }

    /* Build the decimal integer whose digits are the bits of m. */
    seq_t result = 0;
    for (idx_t i = bits; i > 0; --i) {
        result = result * 10 + ((m >> (i - 1)) & 1);
    }

    return result;
}
