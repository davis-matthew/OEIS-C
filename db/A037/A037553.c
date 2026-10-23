/*
 * A037553  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,1.
 * https://oeis.org/A037553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; a(n)=4*a(n-1)+digit_n,
   where digit_n repeats 2,1,1 in that order. */
seq_t a(idx_t n) {
    seq_t val = 0;
    static const seq_t pattern[3] = {2, 1, 1};
    for (idx_t i = 1; i <= n; ++i) {
        seq_t d = pattern[(i - 1) % 3];
        val = val * 4 + d;
    }
    return val;
}
