/*
 * A100050  A Chebyshev transform of n.
 * https://oeis.org/A100050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(3n) = 0, a(3n+1) = (-1)^n * (3n+1), a(3n+2) = (-1)^n * (3n+2)
 * Range:     i8 n <= 127, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* using the explicit formula gives an O(1) implementation */
    if ((n % 3) == 0u) {
        return (seq_t)0;
    }
    /* n = 3k + r, r = 1 or 2.  Sign depends on parity of k = n/3. */
    idx_t k = n / 3;
    seq_t val = (seq_t)n;
    seq_t sign = ((k & 1u) == 0u) ? 1 : -1;
    return sign * val;
}
