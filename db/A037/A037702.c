/*
 * A037702  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0,2.
 * https://oeis.org/A037702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the integer whose base‑4 representation is the *
 *            concatenation of the first n terms of the periodic sequence *
 *            1,3,0,2. Thus a(1)=1, a(2)=13_4=7, a(3)=130_4=28, etc
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[4] = {(seq_t)1, (seq_t)3, (seq_t)0, (seq_t)2};
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i)
        res = res * (seq_t)4 + pat[(i - 1) & 3];
    return res;
}
