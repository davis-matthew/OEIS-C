/*
 * A168269  a(n) = 2*n - (-1)^n.
 * https://oeis.org/A168269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - (-1)^n
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = ((seq_t)n << 1);   /* res = 2*n */
    if ((n & 1) == 0)              /* n even: subtract 1   */
        res -= 1;
    else                           /* n odd : add 1        */
        res += 1;
    return res;
}
