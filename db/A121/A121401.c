/*
 * A121401  a(n) = ((sqrt(3)+1)^n+(sqrt(3)-1)^n)^2/2^(n+1).
 * https://oeis.org/A121401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3); a(0)=2, a(1)=3, a(2)=8
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;

    /* Compute iteratively using the recurrence */
    seq_t a0 = (seq_t)2;   /* a(n-3) */
    seq_t a1 = (seq_t)3;   /* a(n-2) */
    seq_t a2 = (seq_t)8;   /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t an = 5 * a2 - 5 * a1 + a0;  /* a(i) = 5*a(i-1) - 5*a(i-2) + a(i-3) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;  /* a(n) */
}
