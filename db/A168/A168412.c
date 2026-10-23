/*
 * A168412  a(n) = n^5*(n^6 + 1)/2.
 * https://oeis.org/A168412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5*(n^6 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 60, u128 n <= 3390,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    /* compute n^5 */
    seq_t n5 = nseq;
    for (idx_t i = 1; i < 5; ++i) {
        n5 *= nseq;
    }
    /* compute n^6 = n^5 * n */
    seq_t n6 = n5 * nseq;

    /* divide the even factor by 2 first to avoid overflow */
    if (nseq & 1) {          /* n odd: (n^6 + 1) is even */
        seq_t part = (n6 + 1) / 2;
        return n5 * part;
    } else {                 /* n even: n^5 is even */
        seq_t part = n5 / 2;
        return part * (n6 + 1);
    }
}
