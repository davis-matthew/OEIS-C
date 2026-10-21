/*
 * A094028  Expansion of 1/((1-x)*(1-100*x)).
 * https://oeis.org/A094028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 100*a(n-1) + 1, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;            /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        result = result * (seq_t)100 + (seq_t)1;
    }
    return result;
}
