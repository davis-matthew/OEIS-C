/*
 * A202827  Expansion of e.g.f.: exp(4*x/(1-x)) / sqrt(1-x^2).
 * https://oeis.org/A202827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)*a(n-1)+(n-1)*(n+3)*a(n-2)-(n-1)*(n-2)^2*a(n-3),
 *            a(0)=1, a(1)=4, a(2)=25
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)25;

    /* Use an iterative recurrence.  The sequence values are stored in
       a0 = a(i-3), a1 = a(i-2), a2 = a(i-1).  We then compute a(i)
       and shift the variables for the next step.  All arithmetic is
       performed in seq_t to avoid overflow for non‑arbitrary‐precision
       builds.  */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)4;  /* a(1) */
    seq_t a2 = (seq_t)25; /* a(2) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t term1 = (seq_t)(i + 3) * a2;

        seq_t term2 = (seq_t)(i - 1) * (i + 3);
        term2 *= a1;

        seq_t term3 = (seq_t)(i - 1);
        term3 *= (i - 2);
        term3 *= (i - 2);
        term3 *= a0;

        ai = term1 + term2 - term3;

        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
