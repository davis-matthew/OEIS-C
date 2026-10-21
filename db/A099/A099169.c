/*
 * A099169  a(n) = (1/n) * Sum_{k=0..n-1} C(n,k) * C(n,k+1) * (n-1)^k.
 * https://oeis.org/A099169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n) * Σ_{k=0}^{n-1} C(n,k) * C(n,k+1) * (n-1)^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (replaces the OEIS definition with an iterative evaluation) */
seq_t a(idx_t n)
{
    /* treat the offset: a(1) = 1 */
    if (n == 0)
        return 0;   /* value below the offset is irrelevant */

    seq_t c      = 1;               /* C(n,0)                       */
    seq_t nextc  = (seq_t)n;        /* C(n,1)                       */
    seq_t power  = 1;               /* (n-1)^0                      */
    seq_t sum    = 0;               /* accumulator for the sum      */

    /* iterate k = 0 … n-1 */
    for (idx_t k = 0; k < n; ++k)
    {
        /* term = C(n,k) * C(n,k+1) * (n-1)^k */
        seq_t term = c * nextc * power;
        sum += term;

        /* prepare values for the next k, if any */
        if (k + 1 < n)
        {
            /* (n-1)^(k+1) */
            power *= (seq_t)(n - 1);

            /* new binomial coefficient:
               C(n,k+2) = C(n,k+1) * (n-(k+1)) / (k+2)                */
            seq_t new_c = nextc;
            seq_t temp  = new_c * (seq_t)(n - (k + 1));
            temp /= (seq_t)(k + 2);

            c      = new_c;
            nextc  = temp;
        }
    }

    /* a(n) = sum / n  (division is exact) */
    return sum / (seq_t)n;
}
