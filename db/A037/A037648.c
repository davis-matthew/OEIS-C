/*
 * A037648  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1.
 * https://oeis.org/A037648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)*6 + p[n], p = {3,0,1} repeating. (a(1)=3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = 0;
    static const seq_t period[3] = {3, 0, 1};
    for (idx_t i = 0; i < n; ++i) {
        val = val * 6 + period[i % 3];
    }
    return val;
}
