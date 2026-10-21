/*
 * A230458  Decimal expansion of Δν_{Cs} in unit s^{-1}; one of the seven
 *          units of the 2019 SI system.
 * https://oeis.org/A230458
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence:  A230458 – Decimal expansion of Δν_{Cs} in unit s^{-1}
 * Offset:   10  (a(10) = first digit of the constant 9192631770)
 * Formula:  a(n) = the (n‑9)-th decimal digit of the integer 9192631770
 *           (digit positions are counted from 1 on the leftmost digit).
 *           For n < 10 or n > 19 the value is defined to be 0.
 */

seq_t a(idx_t n)
{
    /* Constant value of Δν_{Cs} */
    const seq_t const_val = (seq_t)9192631770ULL;

    /* The sequence only has 10 meaningful digits (indices 10..19). */
    if (n < 10 || n > 19) {
        return 0;   /* values outside the defined range are 0 */
    }

    /* Number of trailing zeros to strip to reach the desired digit. */
    idx_t shift = 19 - n;   /* shift is 0 for the last digit, 9 for the first */

    seq_t v = const_val;
    while (shift--) {  /* strip shift least significant decimal digits */
        v /= 10;
    }

    return v % 10;   /* return the selected decimal digit */
}
