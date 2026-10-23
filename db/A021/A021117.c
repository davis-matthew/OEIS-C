/*
 * A021117  Decimal expansion of 1/113.
 * https://oeis.org/A021117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/113) mod 10
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the nth digit after the decimal point in 1/113. */

seq_t a(idx_t n) {
    seq_t r = 1;                       /* r = 10^0 mod 113 = 1          */
    for (idx_t i = 0; i < n; ++i) {    /* compute r = 10^n mod 113    */
        r = (r * 10) % 113;
    }
    return (r * 10) / 113;              /* next digit from remainder     */
}
