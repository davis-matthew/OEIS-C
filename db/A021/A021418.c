/*
 * A021418  Decimal expansion of 1/414.
 * https://oeis.org/A021418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the n-th digit of the decimal expansion of 1/414
   (0‑based after the decimal point).  For a given n the digit is
   obtained by the usual long‑division process:
     start with remainder 1,
     repeat n+1 times:
        remainder *= 10
        digit     = remainder / 414
        remainder = remainder % 414
   The final digit is the required term.  All operations stay
   within 64 bits for n ≤ 10000, so the routine runs in O(n)
   time irrespective of the call order. */
seq_t a(idx_t n)
{
    seq_t rem = 1;       /* current remainder of 1 divided by 414 */
    seq_t digit = 0;     /* digit produced at each step */

    for (idx_t k = 0; k <= n; ++k)
    {
        rem *= 10;               /* shift one decimal place */
        digit = rem / 414;       /* integer part is the next digit */
        rem   = rem % 414;       /* keep the remainder for next step */
    }
    return digit;
}
