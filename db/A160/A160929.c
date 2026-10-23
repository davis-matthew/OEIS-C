/*
 * A160929  a(n)= n * reversal(n-1).
 * https://oeis.org/A160929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * reversal(n-1)
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reverse the decimal representation of n-1. */
    idx_t m = n - 1;
    idx_t r = 0;
    while (m > 0) {
        r = r * 10 + (m % 10);
        m /= 10;
    }
    /* Multiply in seq_t to ensure correct size. */
    return (seq_t)n * (seq_t)r;
}
