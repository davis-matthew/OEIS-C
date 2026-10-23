/*
 * A049785  a(n) = Sum_{k=1..n} (n mod floor(n/k)) = T(n,n), array T as in
 *          A049783.
 * https://oeis.org/A049785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n mod floor(n/k))
 * Range:     u8 n <= 178, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; )
    {
        idx_t m   = n / i;          /* floor(n / i) */
        idx_t i2  = n / m;          /* last i with that floor value */
        idx_t cnt = i2 - i + 1;     /* how many ks in this block */
        seq_t mod = N % (seq_t)m;   /* n mod m */
        sum += mod * (seq_t)cnt;
        i = i2 + 1;
    }
    return sum;
}
