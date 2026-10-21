/*
 * A052556  Expansion of e.g.f. 1/(1-x-x^3).
 * https://oeis.org/A052556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*a(n-1) + n*(n-1)*(n-2)*a(n-3) with a(0) = 1, a(1) = 1,
 *            a(2) = 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A052556: Expansion of e.g.f. 1/(1-x-x^3). */
seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Keep last three terms in a circular buffer. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */

    for (idx_t k = 3; k <= n; ++k)
    {
        seq_t nk    = (seq_t)k;
        seq_t nk1   = (seq_t)(k - 1);
        seq_t nk2   = (seq_t)(k - 2);
        seq_t term  = nk * a2                          /* n * a(n-1) */
                    + nk * nk1 * nk2 * a0;                /* n*(n-1)*(n-2)*a(n-3) */
        /* Rotate the buffer: a0 <- a1, a1 <- a2, a2 <- term */
        a0 = a1;
        a1 = a2;
        a2 = term;
    }

    return a2; /* a(n) is now in a2 after the loop */
}
