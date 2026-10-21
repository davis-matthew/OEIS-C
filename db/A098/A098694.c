/*
 * A098694  Double-superfactorials: a(n) = Product_{k=1..n} (2k)!.
 * https://oeis.org/A098694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1..n} (2k)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Computes by iteratively extending the running factorial and product.) */
seq_t a(idx_t n)
{
    seq_t prod = 1;    /* a(n) */
    seq_t fact = 1;    /* (2k)! for current k */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* multiply fact by (2k-1) and (2k) to get (2k)! */
        seq_t t1 = (seq_t)2 * k - 1;
        fact *= t1;                       /* fact *= (2k-1) */
        seq_t t2 = (seq_t)2 * k;
        fact *= t2;                       /* fact *= (2k) */

        prod *= fact;                     /* prod *= (2k)! */
    }

    return prod;
}
