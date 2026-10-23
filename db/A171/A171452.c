/*
 * A171452  a(n) = C(n,2) + floor(n/3).
 * https://oeis.org/A171452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)/2 + n/3 (i.e., C(n,2) + floor(n/3))
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n >= 2)                 /* avoid underflow for n < 2 */
    {
        seq_t sn = (seq_t)n;
        res = sn * (sn - 1) / 2; /* C(n,2) */
    }
    res += (seq_t)n / 3;        /* floor(n/3) */
    return res;
}
