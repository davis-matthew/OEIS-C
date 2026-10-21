/*
 * A218328  Odd 9-gonal (nonagonal) pyramidal numbers.
 * https://oeis.org/A218328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(4*n-3)*(28*n-25)/3
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 (offset 1) */
    seq_t nseq = (seq_t)n;

    seq_t t1 = 2 * nseq - 1;
    seq_t t2 = 4 * nseq - 3;
    seq_t t3 = 28 * nseq - 25;

    /* Use two multiplications to avoid unnecessary overflow in intermediate
       steps, using seq_t arithmetic only. */
    seq_t prod = (t1 * t2) * t3;
    return prod / 3;
}
