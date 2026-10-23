/*
 * A037763  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1,2.
 * https://oeis.org/A037763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the integer whose base‑9 expansion consists of the first
 *            n digits of the recurring pattern 3,0,1,2. That is, a(1)=3,
 *            a(2)=27, a(3)=244, etc
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const int _a037763_pattern[4] = {3, 0, 1, 2};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* offset 1 – value for n<1 irrelevant */
    seq_t value = 0;
    for (idx_t i = 1; i <= n; ++i) {
        int d = _a037763_pattern[(i - 1) & 3U];   /* (i-1) mod 4 */
        value = value * 9 + (seq_t)d;
    }
    return value;
}
