/*
 * A021200  Decimal expansion of 1/196.
 * https://oeis.org/A021200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth digit after the decimal point in the expansion of
 *            1/196 (offset 0). This is computed by performing long division
 *            of 1 by 196
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t rem   = 1;   /* current remainder of 1 / 196  */
    seq_t digit = 0;   /* digit produced in this step   */

    /* Perform long division steps for i = 0 .. n.
       Each iteration produces the digit a(i). */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t prod = rem * 10;   /* shift remainder by one decimal place */
        digit     = prod / 196;  /* integer part is the next digit     */
        rem       = prod % 196;  /* new remainder                       */
    }
    return digit;
}
