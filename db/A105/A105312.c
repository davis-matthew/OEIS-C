/*
 * A105312  a(n) = Sum_{j=0..15} n^j.
 * https://oeis.org/A105312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..15} n^j
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 19, u128 n <= 370,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;          /* n^0 */
    for (int i = 0; i < 16; ++i) {
        result += power;
        power  *= (seq_t)n;   /* n^(i+1) for next iteration */
    }
    return result;
}
