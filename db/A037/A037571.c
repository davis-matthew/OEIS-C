/*
 * A037571  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,2.
 * https://oeis.org/A037571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the base‑6 number whose first n digits are the repeating
 *            pattern 2,1,2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val   = 0;
    const seq_t digits[3] = { 2, 1, 2 };

    for (idx_t i = 0; i < n; ++i)
    {
        val = val * 6 + digits[i % 3];
    }

    return val;
}
