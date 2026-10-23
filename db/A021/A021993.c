/*
 * A021993  Decimal expansion of 1/989.
 * https://oeis.org/A021993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   digit a(n) is the nth decimal digit of 1/989 (0‑based)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 1;          /* current remainder */
    seq_t digit = 0;      /* the digit produced on the last iteration */

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;                         /* shift left one decimal place */
        digit = r / 989;                 /* quotient is the next digit */
        r = r % 989;                     /* new remainder */
    }
    return digit;
}
