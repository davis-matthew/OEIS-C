/*
 * A160546  Not divisible by 29.
 * https://oeis.org/A160546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-28) - a(n-29) (for n > 29) which is
 *            equivalent to a(n) = n + floor((n-1)/28) for all n >= 1. This
 *            gives the sequence of positive integers not divisible by 29
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; undefined below, but guard. */

    /* Convert index to seq_t for arithmetic. */
    seq_t sn = (seq_t)n;
    /* Compute floor((n-1)/28) using seq_t division. */
    seq_t skip = ((seq_t)(n - 1)) / 28;
    return sn + skip;
}
