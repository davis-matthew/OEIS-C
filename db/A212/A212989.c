/*
 * A212989  Number of (w,x,y) with all terms in {0,...,n} and 4*w = 4*x+y.
 * https://oeis.org/A212989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n/4)+1)*(n+1) - ((floor(n/4))*(floor(n/4)+1))/2
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a closed‑form computation for OEIS A212989: */
/* a(n) = # of triples (w,x,y) with w,x,y∈[0,n] and 4*w = 4*x + y. */
seq_t a(idx_t n)
{
    /* D = floor(n/4) */
    idx_t D_idx = n / 4;
    seq_t D = (seq_t)D_idx;

    seq_t n_plus_one = (seq_t)n + (seq_t)1;
    seq_t term1 = (D + (seq_t)1) * n_plus_one;
    seq_t term2 = (D * (D + (seq_t)1)) / (seq_t)2;
    return term1 - term2;
}
