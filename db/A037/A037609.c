/*
 * A037609  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3.
 * https://oeis.org/A037609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the number whose base‑9 digits are the first n terms of
 *            the repeating sequence 1,2,3,1,2,3,… (a(1)=1, a(2)=11, a(3)=102,
 *            etc.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        val = val * (seq_t)9 + (seq_t)((i % 3) + 1);
    }
    return val;
}
