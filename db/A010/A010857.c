/*
 * A010857  Constant sequence: a(n) = 18.
 * https://oeis.org/A010857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    (void)n;               /* silence unused-parameter warning */
    return (seq_t)18L;     /* constant value for all n */
}
