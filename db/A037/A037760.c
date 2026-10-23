/*
 * A037760  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1,2.
 * https://oeis.org/A037760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the number whose base‑6 representation consists
   of the first n digits of the periodic sequence 3,0,1,2,3,0,1,2,... */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t val = 0;
    static const unsigned int period[4] = {3, 0, 1, 2};
    for (idx_t i = 0; i < n; ++i) {
        val = val * 6 + period[i & 3];
    }
    return val;
}
