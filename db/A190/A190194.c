/*
 * A190194  a(n) = Sum_{r=0..n-1} Sum_{s=r+1..n} s! * r^(n-s).
 * https://oeis.org/A190194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r=0..n-1} Sum_{s=r+1..n} s! * r^(n-s)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0, and the above summation is valid for n > 0 */
    if (n == 0)
        return 0;

    seq_t total = 0;
    /* fact_r holds r! for the current outer loop iteration */
    seq_t fact_r = 1;          /* 0! = 1 */

    for (idx_t r = 0; r < n; ++r)
    {
        idx_t m   = n - r;     /* number of terms in the inner sum */
        seq_t t   = 0;
        seq_t f   = fact_r;    /* starts as r! */

        /* inner loop: compute T(r,m) = ∑_{k=1}^{m} (r+k)! * r^{m-k} */
        /* We build the factorials incrementally: fact of (r+k) = f * (r+k) */
        for (idx_t k = 1; k <= m; ++k)
        {
            f = f * (seq_t)(r + k);          /* (r+k)!   */
            t = t * (seq_t)r + f;            /* r * t + (r+k)! */
        }

        total += t;

        /* advance fact_r to (r+1)! for the next outer iteration */
        fact_r = fact_r * (seq_t)(r + 1);
    }

    return total;
}
