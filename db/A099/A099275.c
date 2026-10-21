/*
 * A099275  Unsigned member r=-17 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A099275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) + 18*a(n-2) - a(n-3), a(0)=0, a(1)=1, a(2)=17
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle very small indices directly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 17;

    /* We need values for a(n-3), a(n-2), a(n-1) to compute a(n). */
    seq_t a0 = 0;    /* a(n-3) */
    seq_t a1 = 1;    /* a(n-2) */
    seq_t a2 = 17;   /* a(n-1) */
    seq_t an = 0;    /* a(n) to be computed */

    /* Iterate up to the requested index. */ 
    for (idx_t i = 3; i <= n; ++i)
    {
        an = 18 * a2 + 18 * a1 - a0;  /* 18*a(n-1) + 18*a(n-2) - a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
