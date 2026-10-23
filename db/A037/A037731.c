/*
 * A037731  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0,3.
 * https://oeis.org/A037731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = base‑5 number whose first n digits are the repeating
 *            pattern 2,1,0,3 Recurrence: a(0)=0;
 *            a(n)=5*a(n-1)+pattern[(n-1)%4], where pattern={2,1,0,3}
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const idx_t pattern[4] = {2, 1, 0, 3};
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        val = val * (seq_t)5 + (seq_t)pattern[i & 3];
    }
    return val;
}
