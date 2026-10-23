/*
 * A021085  Decimal expansion of 1/81.
 * https://oeis.org/A021085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = (n % 9 == 8) ? 9 : (n % 9)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (fits the decimal expansion of 1/81, repeating pattern 0,1,2,3,4,5,6,7,9) */
seq_t a(idx_t n) {
    idx_t m = n % 9;
    if (m == 8) return (seq_t)9;
    return (seq_t)m;
}
