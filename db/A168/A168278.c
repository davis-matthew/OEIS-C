/*
 * A168278  a(n) = (10*n + 5*(-1)^n + 3)/4.
 * https://oeis.org/A168278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 5*(-1)^n + 3)/4
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the integer type used for sequence values. */
    seq_t nseq = (seq_t)n;

    /* n % 2, used to encode (-1)^n as 1 for even, -1 for odd. */
    seq_t parity = (seq_t)(n & (idx_t)1);

    /* Compute the numerator: 10*n + 8 - 10*parity
       This is equivalent to 10*n + 5*(-1)^n + 3. */
    seq_t numer = 10 * nseq + 8 - 10 * parity;

    /* Integer division by 4 yields the term value. */
    return numer / 4;
}
