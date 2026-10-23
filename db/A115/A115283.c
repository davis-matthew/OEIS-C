/*
 * A115283  Diagonal sums of correlation triangle for 3-2*0^n.
 * https://oeis.org/A115283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum{k=0..floor(n/2), sum{j=0..n-k,
 *            [j<=k]*(3-2*0^(k-j))*[j<=n-2k]*(3-2*0^(n-2k-j))}}
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 2;           /* floor(n/2) */
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t term;
        if (k <= n / 3)          /* k <= floor(n/3) : first case */
        {
            if (n == 3 * k)
                term = 9 * (seq_t)k + 1;
            else
                term = 9 * (seq_t)k + 3;
        }
        else                     /* k > floor(n/3) : second case */
        {
            idx_t t = n - 2 * k; /* n - 2k  (non‑negative) */
            term = 9 * (seq_t)t + 3;
        }
        sum += term;
    }
    return sum;
}
