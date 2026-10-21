/*
 * A066256  a(n) = k that produces the smallest integer m > 1 of the form m =
 *          (k^2 + n)/(n^2 + k).
 * https://oeis.org/A066256
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k that produces the smallest integer m > 1 of the form m
 *            = (k^2 + n)/(n^2 + k)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation scans k upward starting from n+1 until the
   divisibility condition holds; k is then returned. */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;              /* undefined for n < 2 */

    seq_t sn  = (seq_t)n;            /* cast to seq_t for arithmetic */
    seq_t sn2 = sn * sn;             /* n^2 */

    /* k must be > n to get m > 1, so start at n+1. */
    for (idx_t k = n + 1; ; ++k) {
        seq_t sk = (seq_t)k;         /* current k as seq_t */

        seq_t numerator   = sk * sk + sn;   /* k^2 + n */
        seq_t denominator = sn2 + sk;       /* n^2 + k */

        /* Because k > n, numerator > denominator, hence m > 1 automatically. */
        if (numerator % denominator == 0)
            return sk;                  /* return the smallest k found */
    }
}
