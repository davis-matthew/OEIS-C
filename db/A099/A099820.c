/*
 * A099820  Even nonnegative integers in base 2 (bisection of A007088).
 * https://oeis.org/A099820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10 * (decimal value of the binary representation of n)
 *            Equivalently, a(n) = 10 * A007088(n) = A007088(2*n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the binary representation is the single digit 0, so
       the decimal conversion is 0 and a(0) = 0.  The algorithm works
       for all n >= 0, and is O(number of bits in n). */
    if (n == 0)
        return (seq_t)0;

    /* Find the position of the most significant bit (0‑based). */
    idx_t tmp = n;
    idx_t msb = 0;
    while (tmp > 1) {
        tmp >>= 1;
        ++msb;
    }

    /* Convert the binary representation to decimal by processing
       bits from most significant to least significant. */
    seq_t res = 0;
    for (idx_t i = msb; i != (idx_t)-1; --i) {
        seq_t bit = (seq_t)((n >> i) & (idx_t)1);
        res = res * 10 + bit;
    }

    /* Append the last digit 0 by multiplying by 10. */
    res *= 10;
    return res;
}
