/*
 * A063233  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          79 ).
 * https://oeis.org/A063233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13n - (6 if n is odd and n > 1 else 7)
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against n < 1 (undefined in OEIS); the behaviour is unspecified. */
    seq_t val = (seq_t)13 * n;
    if (n > 1 && (n & 1)) {
        val -= 6;
    } else {
        val -= 7;
    }
    return val;
}
