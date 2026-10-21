/*
 * A212969  Number of (w,x,y) with all terms in {0,...,n} and w != x and x >
 *          range(w,x,y).
 * https://oeis.org/A212969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(2*n*(7*n-2) + (n%2?6:0))/24
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A212969 */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t N   = (seq_t)n;          /* cast index to sequence type */
    seq_t nm1 = N - 1;             /* n-1, safe here because n>=2 */

    /* compute t = 2*n*(7*n-2) + (n odd? 6 : 0) */
    seq_t t = ((seq_t)7 * N - 2) * 2 * N;   /* 2*n*(7*n-2) */
    if (n & 1)
        t += 6;                       /* add 6 for odd n */

    seq_t numerator = nm1 * t;      /* (n-1)*t  */
    return numerator / 24;          /* division is exact by the formula */
}
