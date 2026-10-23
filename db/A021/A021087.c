/*
 * A021087  Decimal expansion of 1/83.
 * https://oeis.org/A021087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = floor(10 * (10^n mod 83) / 83)
   gives the n‑th decimal digit of 1/83 after the decimal point
   (a(0) = 0, a(1) = 1, a(2) = 2, …) */
seq_t a(idx_t n)
{
    seq_t r = 1;  /* current remainder (mod 83) */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        r = r * (seq_t)10;          /* shift left one decimal place */
        digit = r / (seq_t)83;      /* next decimal digit */
        r = r - digit * (seq_t)83;  /* new remainder */
    }
    return digit;
}
