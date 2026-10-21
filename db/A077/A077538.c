/*
 * A077538  First differences of triangular numbers with square pyramidal
 *          indices.
 * https://oeis.org/A077538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)² * (n+2) * (2*n² + 2*n + 3)) / 6
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 103, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic */
    seq_t nseq = (seq_t)n;

    /* (n+1), (n+2) and (n+1)² */
    seq_t n1   = nseq + 1;
    seq_t n2   = n1 + 1;
    seq_t nsq  = n1 * n1;

    /* 2*n² + 2*n + 3, computed entirely in seq_t */
    seq_t t = (seq_t)2 * nseq * nseq + (seq_t)2 * nseq + 3;

    /* Multiply and divide, division is exact */
    seq_t prod = nsq * n2;   /* (n+1)² * (n+2) */
    prod       = prod * t;   /* * (2*n² + 2*n + 3) */
    prod       = prod / 6;   /* /6 */

    return prod;
}
