/*
 * A024932  a(n) = Sum_{k=1..n} k*[ (n/k)*[ n/k ] ].
 * https://oeis.org/A024932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * floor( (n/k) * floor(n/k) )
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t t = n / k;                       /* floor(n/k) */
        seq_t nt = (seq_t)n * (seq_t)t;        /* n * t */
        seq_t m  = nt / (seq_t)k;              /* floor( (n*t)/k ) */
        sum += (seq_t)k * m;                   /* k * m */
    }
    return sum;
}
