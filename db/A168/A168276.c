/*
 * A168276  a(n) = 2*n - (-1)^n - 1.
 * https://oeis.org/A168276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - (-1)^n - 1
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * 2;
    if ((n & 1) == 0) { /* n is even */
        val -= 2;
    }
    return val;
}
