/*
 * A160094  a(n) = 1 + A122840(n).
 * https://oeis.org/A160094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1 + (number of trailing zeros in the decimal *
 *            representation of n), for n ≥ 1. The count is zero for n that *
 *            are not a multiple of 10, so a(n) = 1 for all such n. For n that
 *            * have k trailing zeros, a(n) = k + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is defined.  For safety, if n==0
     * return 0 – the value for indices below the offset does not
     * matter. */
    if (n == 0)
        return 0;

    /* Count trailing zeros in base 10.  Since n is at most 64-bit,
     * the loop executes at most 19 times. */
    idx_t temp = n;
    idx_t tz   = 0;
    while ((temp % 10) == 0) {
        ++tz;
        temp /= 10;
    }

    /* Return the answer; cast to seq_t. */
    return (seq_t)tz + 1;
}
