/*
 * A026534  a(n) = Sum_{i=0..2*n} Sum_{j=0..n-1} A026519(j, i).
 * https://oeis.org/A026534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..2*n} Sum_{j=0..n-1} A026519(j, i)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset 1.  a(1) is the first term. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)10;

    /* Recurrence derived from the rational generating function
       g(x) = x(1+3x)/((1-x)(1-6x^2)).
       a(n) = a(n-1) + 6*a(n-2) - 6*a(n-3)   (for n ≥ 4)      */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)4;   /* a(2) */
    seq_t a3 = (seq_t)10;  /* a(3) */

    for (idx_t k = 4; k <= n; ++k)
    {
        seq_t diff = a2 - a1;
        seq_t next = a3 + (seq_t)6 * diff;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
