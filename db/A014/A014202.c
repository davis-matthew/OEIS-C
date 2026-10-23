/*
 * A014202  Number of solutions to x^2 + x*y + y^2 <= n, excluding (0,0),
 *          divided by 6.
 * https://oeis.org/A014202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} K(k,3)*floor(n/k), where K(k,3)=1 if k≡1
 *            (mod3), K(k,3)=−1 if k≡2 (mod3), and K(k,3)=0 if k≡0 (mod3)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    long long sum = 0;          /* signed intermediate sum; result fits easily */
    for (idx_t k = 1; k <= n; ++k)
    {
        int s = 0;
        switch (k % 3)
        {
            case 1: s = 1; break;
            case 2: s = -1; break;
            /* case 0: s stays 0 */
        }
        if (s != 0)
            sum += s * (long long)(n / k);
    }
    return (seq_t)sum;
}
