/*
 * A021098  Decimal expansion of 1/94.
 * https://oeis.org/A021098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10*(10^n mod 94) / 94) – the (n+1)-th digit of the
 *            decimal expansion of 1/94, computed by long division: each step
 *            sets remainder = (remainder * 10) % 94; digit = (remainder * 10)
 *            / 94
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned int rem  = 1U;    /* remainder after dividing 1 by 94   */
    unsigned int digit = 0U;   /* current digit extracted            */

    for (idx_t i = 0; i <= n; ++i) {
        unsigned int prod = rem * 10U;   /* 0 ≤ prod < 940, fits in unsigned int */
        digit = prod / 94U;              /* digit of this position             */
        rem   = prod % 94U;              /* new remainder .                    */
    }

    return (seq_t)digit;
}
