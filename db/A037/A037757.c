/*
 * A037757  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,3,1,0.
 * https://oeis.org/A037757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the integer formed by concatenating the first n terms of
 *            the repeating sequence 2,3,1,0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int pattern[4] = {2, 3, 1, 0};
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        result = result * 10 + pattern[(i - 1) & 3];
    }
    return result;
}
