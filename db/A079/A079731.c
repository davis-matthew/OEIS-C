/*
 * A079731  Fundamental piano frequencies in Hertz rounded to nearest integer.
 * https://oeis.org/A079731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = round(27.5 * 2^n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) {
        return (seq_t)28;
    }

    /* For n >= 1 : a(n) = 55 * 2^(n-1)  (exact integer, no rounding needed) */
    seq_t val = (seq_t)55;
    for (idx_t i = 1; i < n; ++i) {
        val = val * (seq_t)2;
    }
    return val;
}
