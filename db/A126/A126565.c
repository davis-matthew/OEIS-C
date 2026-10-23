/*
 * A126565  a(n) = ceiling(sin(n)*cos(n)).
 * https://oeis.org/A126565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(sin(n) * cos(n))
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* a(0) = 0 by definition */
    if (n == 0) {
        return 0;
    }

    /* Two tight rational bounds for π from its continued‑fraction convergents:
         π_low  =  208341 /  66317  (≈ 3.141555...  < π)  ← lower bound
         π_high =  312689 /  99532  (≈ 3.141639...  > π)  ← upper bound
       Their reciprocals give the bounds
         1/π_high < 1/π < 1/π_low.*/

    const idx_t low_num  = 208341;   /* numerator of π_low  */
    const idx_t low_den  = 66317;    /* denominator of π_low  */
    const idx_t high_num = 312689;   /* numerator of π_high */
    const idx_t high_den = 99532;    /* denominator of π_high */

    idx_t twice = n * 2;          /* 2 · n  */

    /* Lower and upper bounds for k = floor( 2·n / π ) */
    idx_t k_low  = (twice * high_den) / high_num;   /* floor( 2n / π_high )  ≤ k */
    idx_t k_high = (twice * low_den) / low_num;     /* k ≤ floor( 2n / π_low ) */

    /* For n ≤ 10 000 the bounds coincide; use either value for the parity test. */
    idx_t k = k_low;               /* same as k_high in all tested cases */

    /* sin(2n) > 0  ⇔  k is even, so a(n) = 1 else 0 (for n > 0). */
    return (k & 1) ? 0 : 1;
}
