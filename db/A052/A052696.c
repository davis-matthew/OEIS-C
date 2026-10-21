/*
 * A052696  Expansion of e.g.f. (1-x)^2/(1-4*x+3*x^2-x^3).
 * https://oeis.org/A052696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n*a(n-1) - 3*n*(n-1)*a(n-2) + n*(n-1)*(n-2)*a(n-3)
 *            Initial values: a(0)=1, a(1)=2, a(2)=12
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)12;

    /* Use three successive values to iterate the recurrence */
    seq_t a0 = (seq_t)1;  /* a(k-3) */
    seq_t a1 = (seq_t)2;  /* a(k-2) */
    seq_t a2 = (seq_t)12; /* a(k-1) */
    seq_t an = 0;        /* a(k) */

    for (idx_t k = 3; k <= n; ++k)
    {
        /* Compute term 4*k*a(k-1) */
        seq_t term1 = (seq_t)(4 * k) * a2;
        /* Compute term 3*k*(k-1)*a(k-2) */
        seq_t term2 = (seq_t)(3 * k * (k - 1)) * a1;
        /* Compute term k*(k-1)*(k-2)*a(k-3) */
        seq_t term3 = (seq_t)(k * (k - 1) * (k - 2)) * a0;

        an = term1 - term2 + term3;

        /* Shift variables for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}
