/*
 * A089835  a(n) = (A000108(n)^2)*(n+1)!.
 * https://oeis.org/A089835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Catalan(n))^2 * (n+1)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 2, a(2) = 24, ... */

seq_t a(idx_t n)
{
    seq_t cat   = (seq_t)1;   /* Catalan(0) = 1          */
    seq_t fact  = (seq_t)1;   /* (n+1)! will accumulate   */

    /* loop i = 1 .. n computing Catalan(i) and (i+1)! */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Catalan(i) = Catalan(i-1) * (4*i-2) / (i+1)   */
        cat = cat * (seq_t)(4*i - 2);
        cat = cat / (seq_t)(i + 1);

        /* (i+1)! = i! * (i+1) */
        fact = fact * (seq_t)(i + 1);
    }

    /* a(n) = Catalan(n)^2 * (n+1)! */
    return cat * cat * fact;
}
