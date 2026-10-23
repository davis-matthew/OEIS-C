/*
 * A021578  Decimal expansion of 1/574.
 * https://oeis.org/A021578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal expansion of 1/574 (performed by
 *            ordinary long division)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t DEN = 574;
    idx_t rem = 1;             /* initial numerator (before decimal point) */
    idx_t i;

    for (i = 0; i <= n; ++i) {
        rem *= 10;
        seq_t d = rem / DEN;      /* the current decimal digit 0..9 */
        rem  %= DEN;              /* remainder for next step */
        if (i == n)
            return d;
    }
    /* This point is never reached, but keeps the compiler happy. */
    return 0;
}
