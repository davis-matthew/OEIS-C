/*
 * A078689  Continued fraction expansion of e^(1/3).
 * https://oeis.org/A078689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   a(3k+1) = 6k+2, otherwise a(n) = 1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 3) == 1) {
        idx_t k = (n - 1) / 3;
        return (seq_t)(6 * k + 2);
    }
    return (seq_t)1;
}
