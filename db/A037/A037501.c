/*
 * A037501  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2.
 * https://oeis.org/A037501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value of the first n digits of the periodic base‑8
 *            pattern 1,0,2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char pattern[3] = {1, 0, 2};

seq_t a(idx_t n)
{
    seq_t result = 0;
    unsigned pos = 0;
    for (idx_t i = 0; i < n; ++i) {
        result = result * 8 + (seq_t)pattern[pos];
        ++pos;
        if (pos == 3) pos = 0;
    }
    return result;
}
