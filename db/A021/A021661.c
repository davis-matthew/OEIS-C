/*
 * A021661  Decimal expansion of 1/657.
 * https://oeis.org/A021661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the nth decimal digit of 1/657 after the decimal point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rem = 1;          /* current remainder before producing a digit */
    idx_t digit = 0;        /* the digit produced in the current iteration */

    for (idx_t i = 0; i <= n; ++i) {
        idx_t tmp = rem * 10;      /* multiply remainder by 10 (bring down a 0) */
        digit = tmp / 657;         /* next decimal digit of 1/657 */
        rem   = tmp % 657;         /* remainder for the next iteration */
    }

    return (seq_t)digit;
}
