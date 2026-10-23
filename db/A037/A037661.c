/*
 * A037661  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0.
 * https://oeis.org/A037661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*a(n-1) + d_n, a(1)=3 where d_n cycles 3,1,0 for n =
 *            3k+1,3k+2,3k respectively
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = 0;
    static const seq_t digits[3] = {3, 1, 0};
    idx_t d = 0;                     // index into digits array
    for (idx_t i = 1; i <= n; ++i) {
        value = value * 5 + digits[d];
        ++d;
        if (d == 3) d = 0;
    }
    return value;
}
