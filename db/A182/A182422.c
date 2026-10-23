/*
 * A182422  a(n) = Sum_{k = 0..n} C(n,k)^8.
 * https://oeis.org/A182422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)^8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t C = (seq_t)1;
    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        /* compute C^8 */
        seq_t t = C * C;          /* C^2 */
        t = t * t;                /* C^4 */
        t = t * t;                /* C^8 */
        sum = sum + t;

        /* update C to next binomial coefficient C(n,k+1) */
        if (k != n)
        {
            C = (C * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }
    return sum;
}
