/*
 * A180104  Floor( 100*n/(n-1) ).
 * https://oeis.org/A180104
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(100*n/(n-1)) (offset: 2)
 * Range:     u8 n <= 2, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 2 by definition of the sequence */
    if (n <= 51) {
        seq_t num = (seq_t)n * 100;
        seq_t den = (seq_t)n - 1;
        return num / den;
    } else if (n <= 101) {
        return 101;
    } else {
        return 100;
    }
}
