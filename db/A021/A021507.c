/*
 * A021507  Decimal expansion of 1/503.
 * https://oeis.org/A021507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/503) mod 10, i.e. the nth decimal digit of
 *            1/503
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t rem = 1;          /* current remainder of the long division */
    idx_t digit = 0;        /* current decimal digit */
    const idx_t divisor = 503;  /* 1/503 */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= (idx_t)10;        /* bring down next decimal digit */
        digit = rem / divisor;   /* compute the digit */
        rem %= divisor;          /* update the remainder */
    }
    return (seq_t)digit;
}
