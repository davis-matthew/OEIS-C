/*
 * A021105  Decimal expansion of 1/101.
 * https://oeis.org/A021105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 9 if n mod 4 = 2 or 3; otherwise 0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = n & 3;
    if (r == 2 || r == 3)
        return (seq_t)9;
    return (seq_t)0;
}
