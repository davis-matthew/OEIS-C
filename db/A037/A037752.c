/*
 * A037752  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1,0.
 * https://oeis.org/A037752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=2, a(n)=5*a(n-1)+period[(n-1)%4] where period=[2,3,1,0]
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[4] = {2, 3, 1, 0};
    if (n == 0) return 0;               /* not used, defensive */
    seq_t val = period[0];               /* a(1) = 2 */
    for (idx_t i = 2; i <= n; ++i) {
        val = val * 5 + period[(i - 1) % 4];
    }
    return val;
}
