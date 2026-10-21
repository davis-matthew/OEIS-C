/*
 * A063428  a(n) is the smallest positive integer of the form n*k/(n+k).
 * https://oeis.org/A063428
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer b < n such that (n-b) divides n*b
 * Range:     u8 n <= 18, u16 n <= 420, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset starts at 2, but for n < 2 we return 0. */
    if (n < 2) return 0;

    seq_t nseq = (seq_t)n;
    for (idx_t b = 1; b < n; ++b) {
        seq_t bseq = (seq_t)b;
        seq_t denom = nseq - bseq;          /* n - b > 0 for b < n */
        seq_t prod  = nseq * bseq;          /* n*b */
        if (prod % denom == 0)
            return bseq;                    /* found minimal b */
    }
    /* Fallback – for prime n the only solution is b = n-1 */
    return nseq - 1;
}
