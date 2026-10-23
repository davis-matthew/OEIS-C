/*
 * A037742  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3,0.
 * https://oeis.org/A037742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose base‑9 digits are the first n terms of the
 *            repeating sequence 2,1,3,0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned char digits[4] = {2,1,3,0};
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        res = res * (seq_t)9 + (seq_t)digits[i & 3];
    }
    return res;
}
