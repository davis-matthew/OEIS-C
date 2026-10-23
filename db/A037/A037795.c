/*
 * A037795  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,1,0.
 * https://oeis.org/A037795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(n-1) + digit_{n-1}, where digit cycles 3, 2, 1, 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base-6 representation: a(1) = 3, a(2) = 20, a(3) = 121, etc.               */

seq_t a(idx_t n)
{
    static const unsigned char pattern[] = {3, 2, 1, 0};
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        res = res * 6 + pattern[i & 3];
    }
    return res;
}
