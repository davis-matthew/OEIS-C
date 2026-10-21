/*
 * A209267  a(n) = 1 + 2*n^2 + 3*n^3 + 4*n^4 + 5*n^5 + 6*n^6 + 7*n^7 + 8*n^8.
 * https://oeis.org/A209267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*n^2 + 3*n^3 + 4*n^4 + 5*n^5 + 6*n^6 + 7*n^7 + 8*n^8
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 12, u64 n <= 197, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;   /* cast index to seq_t once */
    seq_t pown = sn;         /* store n^1 initially (will be multiplied to get n^k) */
    seq_t sum  = 1;          /* constant term */

    /* add the terms from k = 2 up to 8 */
    for (idx_t k = 2; k <= 8; ++k) {
        pown *= sn;                      /* compute n^k */
        sum += (seq_t)k * pown;          /* add k*n^k */
    }

    return sum;
}
