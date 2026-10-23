/*
 * A037682  Base-5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3,2.
 * https://oeis.org/A037682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base‑5 number formed by the first n terms of the periodic
 *            sequence 1,0,3,2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int pat[4] = {1, 0, 3, 2};
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        res = res * 5 + pat[i & 3];
    }
    return res;
}
