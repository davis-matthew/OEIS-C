/*
 * A033386  a(n) = floor(66/n).
 * https://oeis.org/A033386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(66/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 66 is constant; cast to seq_t to ensure correct integer type */
    return (seq_t)66 / (seq_t)n;
}
