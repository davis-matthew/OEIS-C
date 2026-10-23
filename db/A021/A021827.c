/*
 * A021827  Decimal expansion of 1/823.
 * https://oeis.org/A021827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th digit after the decimal point of 1/823, with n =
 *            0,1,2,
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A021827:  Decimal expansion of 1/823.
    a(0) is the first digit after the decimal point of 1/823.
*/ 

seq_t a(idx_t n)
{
    /*  Use long‑division: start with remainder 1 (the numerator)
        and repeatedly multiply by 10, divide by 823, and keep the remainder.
    */
    seq_t remainder = 1;
    seq_t digit = 0;               /* holds the current digit */

    for (idx_t i = 0; i <= n; ++i) {
        remainder *= (seq_t)10;            /* shift decimal point right */
        digit   = remainder / (seq_t)823;  /* current decimal digit  */
        remainder = remainder % (seq_t)823;/* new remainder for next step */
    }

    return digit;   /* digit ∈ {0,…,9} */
}
