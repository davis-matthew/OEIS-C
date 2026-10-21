/*
 * A085744  a(n) = A000217(n^3) - n^3.
 * https://oeis.org/A085744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * (n^3 - 1) / 2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 45, u64 n <= 1824,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first to avoid truncation */
    seq_t nsq = (seq_t)n;

    /* Compute n^3, intermediates fit for n up to the limit where
       the final result fits into seq_t.  Each multiplication is
       performed in seq_t, so any overflow only affects values that
       do not fit in seq_t, which are not verified. */
    seq_t n3 = nsq * nsq * nsq;

    /* One of n3 and (n3-1) is even.  We divide the even factor by
       two first to keep intermediate products from overflowing
       seq_t when it can hold the final result.  The parity of n3
       is the same as the parity of n, so we test n directly to
       avoid computing n3 parity on possibly large numbers. */
    if ((n & 1) == 0) {
        /* n is even → n3 is even. */
        return (n3 / 2) * (n3 - 1);
    } else {
        /* n is odd → n3-1 is even. */
        return n3 * ((n3 - 1) / 2);
    }
}
