/*
 * A021910  Decimal expansion of 1/906.
 * https://oeis.org/A021910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit n of the decimal expansion of 1/906 * (i.e., the
 *            (n+1)-th digit after the decimal point of 1/906)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t DEN = 906;          /* denominator of the fraction 1/906   */
    idx_t      r = 1;               /* current remainder before producing a digit   */
    seq_t     digit = 0;            /* digit computed in the current step          */

    /* Perform long division to obtain the n-th digit.
     * The loop is written as an infinite loop with an explicit
     * return when the required digit index is reached.
     * Only integral arithmetic is used; no floating point.  */
    for (idx_t k = 0; ; ++k)
    {
        /* compute next digit and update the remainder */
        idx_t prod = r * 10;
        digit = (seq_t)(prod / DEN);      /* floor division   */
        r = prod % DEN;                   /* new remainder    */
        if (k == n)            /* desired digit reached */
            return digit;
    }
}
