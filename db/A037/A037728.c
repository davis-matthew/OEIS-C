/*
 * A037728  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3,1.
 * https://oeis.org/A037728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 2
   a(n) = 9 * a(n-1) + d(n), where d(n) repeats 2,0,3,1.
   That is d(1)=2, d(2)=0, d(3)=3, d(4)=1, d(5)=2, ...
*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static const unsigned char digits[4] = {2, 0, 3, 1};
    seq_t val = 0;
    idx_t i;
    for (i = 1; i <= n; ++i)
    {
        val = val * 9 + (seq_t)digits[(i - 1) & 3];
    }
    return val;
}
