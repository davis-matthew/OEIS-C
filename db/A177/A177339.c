/*
 * A177339  Partial sums of round(n^2/44).
 * https://oeis.org/A177339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((2*n+1)*(2*n^2 + 2*n - 15)/528)
 * Range:     u8 none, u16 n <= 24, u32 n <= 1493, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        = floor(((2*n+1)*(2*n^2 + 2*n - 15)+264)/528) */
seq_t a(idx_t n) {
    seq_t n_ts   = (seq_t)n;               /* cast index to seq_t          */
    seq_t term1  = (seq_t)2 * n_ts + (seq_t)1;
    /* 2*n^2 + 2*n - 15.  Use seq_t arithmetic throughout. */
    seq_t term2  = (seq_t)2 * n_ts * n_ts + (seq_t)2 * n_ts - (seq_t)15;
    seq_t num    = term1 * term2;          /* product */
    num += (seq_t)264;                    /* add half of the divisor for rounding */
    return num / (seq_t)528;              /* integer division gives rounded value */
}
