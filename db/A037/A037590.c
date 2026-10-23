/*
 * A037590  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3.
 * https://oeis.org/A037590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: the base‑4 digits are the first n terms of the periodic
   sequence with initial period 1,0,3.  a(n) is the base‑4 value
   represented by those n digits.  */
seq_t a(idx_t n)
{
    static const unsigned char pat[3] = {1,0,3};
    seq_t v = 0;
    for (idx_t i = 0; i < n; ++i)
        v = v * 4 + (seq_t)pat[i % 3];
    return v;
}
