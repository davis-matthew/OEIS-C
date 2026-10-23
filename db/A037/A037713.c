/*
 * A037713  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2,0.
 * https://oeis.org/A037713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose base‑8 digits are the first n terms of the
 *            repeating sequence 1,3,2,0 (period 4). I.e. a(1)=1, a(2)=13_8,
 *            a(3)=132_8, …
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t pat[4] = {1, 3, 2, 0};
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        res = res * 8 + pat[i & 3];
    }
    return res;
}
