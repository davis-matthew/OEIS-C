/*
 * A021042  Decimal expansion of 1/38.
 * https://oeis.org/A021042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0) = 0; for n > 0, a(n) is the (n‑1)th digit in the repeating
 *            block 2,6,3,1,5,7,8,9,4,7,3,6,8,4,2,1,0,5 of the decimal
 *            expansion of 1/38. This generates the sequence exactly as in
 *            A021042
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed repeating block of digits for 1/38 after the initial
       terminating zero.  The block length is 18. */
    static seq_t period[18];
    static int period_len = 0;

    /* Build the block only once. */
    if (period_len == 0) {
        idx_t rem = 1;                     /* initial remainder */
        /* Skip the first digit (which is zero and gives the terminating
           factor of 2 in the denominator).  After this division we
           have the remainder that starts the repeating cycle. */
        rem = (rem * 10) % 38;             /* remainder becomes 10 */

        idx_t i = 0;
        while (i < 18) {
            /* Compute next decimal digit: floor((rem * 10) / 38). */
            seq_t digit = (seq_t)((rem * 10) / 38);
            period[i] = digit;
            rem = (rem * 10) % 38;
            ++i;
        }
        period_len = 18;
    }

    if (n == 0)
        return (seq_t)0;   /* first term of the sequence */

    /* Map n>0 to the repeating block, with offset 1. */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
