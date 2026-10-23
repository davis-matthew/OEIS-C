/*
 * A102909  a(n) = Sum_{j=0..8} n^j.
 * https://oeis.org/A102909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..8} n^j
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 1;            /* n^0 */
    seq_t pow = 1;            /* current n^k */
    for (int i = 1; i <= 8; ++i) {
        pow = pow * (seq_t)n; /* n^i */
        sum = sum + pow;      /* accumulate */
    }
    return sum;
}
