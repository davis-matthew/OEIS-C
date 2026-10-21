/*
 * A053103  a(n) = ((6*n+10)(!^6))/10(!^6), related to A034724
 *          (((6*n+4)(!^6))/4 sextic, or 6-factorials).
 * https://oeis.org/A053103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((6*n+10)(!^6))/10(!^6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation skips the factors 10 and 4, which cancel the division by 40. */
seq_t a(idx_t n)
{
    /* compute the starting term of the 6‑factorial */
    seq_t start = (seq_t)n * 6 + 10;   /* 6*n + 10 */

    seq_t prod = 1;                   /* accumulating product */

    /* Number of terms in the 6‑factorial: n + 2 (from 6*n+10 down to 4). */
    idx_t last = n + 1;               /* iterate over indices 0 .. n+1 */

    for (idx_t j = 0; j <= last; ++j) {
        seq_t term = start - (seq_t)j * 6;   /* current factor */
        if (term == 10 || term == 4)       /* skip factors that cancel the divisor 40 */
            continue;
        prod *= term;
    }

    return prod;
}
