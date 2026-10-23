/*
 * A105928  a(n) = ((n^3 - 4n + 1)*A000166(n) + (-1)^(n+1)*(n-1)^2) / 6.
 * https://oeis.org/A105928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n^3 - 4n + 1)*A000166(n) + (-1)^(n+1)*(n-1)^2)/6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Based on the sub‑factorial recurrence:   !0 = 1, !1 = 0, !k = (k-1)*( !(k-1)+!(k-2) ) */

seq_t a(idx_t n)
{
    /* Compute the sub‑factorial !n iteratively. */
    seq_t der0 = (seq_t)1;          /* !0 */
    seq_t der1 = (seq_t)0;          /* !1 */
    seq_t dern = 0;                 /* placeholder for !n */

    if (n == 0)
        dern = der0;
    else if (n == 1)
        dern = der1;
    else
    {
        seq_t prevprev = der0;     /* !k-2 */
        seq_t prev     = der1;     /* !k-1 */

        for (idx_t k = 2; k <= n; ++k)
        {
            /* derk = (k-1) * (prev + prevprev) */
            dern = (seq_t)(k - 1) * (prev + prevprev);
            prevprev = prev;
            prev = dern;
        }
    }

    /* n^3 term */
    seq_t tn   = (seq_t)n;
    seq_t n3   = tn * tn * tn;

    /* (n^3 - 4n + 1) * !n */
    seq_t termA = n3 - (seq_t)4 * tn + (seq_t)1;
    seq_t term1 = termA * dern;

    /* (n-1)^2, using a signed intermediate to avoid underflow for n=0 or 1 */
    long long nm1 = (long long)n - 1;
    seq_t sq = (seq_t)(nm1 * nm1);

    /* Add or subtract depending on the parity of n+1 */
    seq_t numerator;
    if ( ((n + 1) & 1ULL) )
        numerator = term1 - sq;    /* (-1)^(n+1) = -1 when n+1 is odd */
    else
        numerator = term1 + sq;    /* (+1) otherwise */

    /* The result is always divisible by 6. */
    return numerator / (seq_t)6;
}
