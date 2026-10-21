/*
 * A067983  Convolution of L(n+1) := A000204(n+1) (Lucas), n>=0, with L(n+5),
 *          n>=0.
 * https://oeis.org/A067983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n+11)*F(n+1) + 11*(n+1)*F(n) (F = Fibonacci)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fn   = 0;   /* F(0) */
    seq_t fn1  = 1;   /* F(1) */

    /* Compute F(n) and F(n+1) iteratively */
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t tmp = fn + fn1;
        fn  = fn1;
        fn1 = tmp;
    }

    seq_t dn = (seq_t)n;
    return ( (18 * dn + 11) * fn1 + 11 * (dn + 1) * fn );
}
