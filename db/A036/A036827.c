/*
 * A036827  a(n) = 26 + 2^(n+1)*(-13 +9*n -3*n^2 +n^3).
 * https://oeis.org/A036827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^k * k^3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 46, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t power = 1;           /* 2^0 */
    for (idx_t k = 0; ; ++k)
    {
        seq_t kk   = (seq_t)k;
        seq_t k3   = kk * kk * kk;   /* k^3 */
        sum += power * k3;           /* add 2^k * k^3 */

        if (k == n) break;          /* last term reached */

        power = power * 2;          /* next power of two */
    }
    return sum;
}
