/*
 * A037697  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3,0.
 * https://oeis.org/A037697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑digit base‑6 number whose digits repeat 1,2,3,0.
 *            Recurrence: a(1)=1; a(n)=a(n-1)*6 + pattern[(n-1) mod 4], where
 *            pattern = {1,2,3,0}
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    static const seq_t pattern[4] = { (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)0 };
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        val = val * (seq_t)6 + pattern[(i - 1) & 3];
    }
    return val;
}
