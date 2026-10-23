/*
 * A037500  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2.
 * https://oeis.org/A037500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + [1,0,2]_{(n-1 mod 3)}, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t pattern[3] = {(seq_t)1, (seq_t)0, (seq_t)2};
    seq_t val = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        val = val * (seq_t)7 + pattern[i % 3];
    }
    return val;
}
