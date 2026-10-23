/*
 * A037766  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,2,1.
 * https://oeis.org/A037766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3, a(n) = 5*a(n-1) + d_{n-1} for n>1, where the repeating
 *            digit sequence d is 3,0,2,1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[4] = {3, 0, 2, 1};
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        val = val * 5 + period[i & 3];  /* i % 4  uses bitmask for speed */
    }
    return val;
}
