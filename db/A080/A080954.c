/*
 * A080954  E.g.f. exp(5x)/(1-x).
 * https://oeis.org/A080954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = n*a(n-1) + 5^n, with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;   /* a(0) */
    seq_t pow5   = 1;   /* 5^0 */
    for (idx_t i = 1; i <= n; ++i) {
        pow5 *= 5;                 /* pow5 = 5^i */
        result = (seq_t)i * result + pow5;  /* a(i) = i*a(i-1)+5^i */
    }
    return result;
}
