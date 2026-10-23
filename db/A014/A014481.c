/*
 * A014481  a(n) = 2^n*n!*(2*n+1).
 * https://oeis.org/A014481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * n! * (2*n+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        result *= (seq_t)i * (seq_t)2;   /* accumulate 2*i for each step   */
    }
    result *= (seq_t)(2 * n + 1);         /* final factor (2n+1)             */
    return result;
}
