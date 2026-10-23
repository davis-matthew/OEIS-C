/*
 * A152889  Periodic sequence [1,0,4,0,0] of period 5.
 * https://oeis.org/A152889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = [1,0,4,0,0][n mod 5]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fixed periodic table (period = 5).  All values are small
       (0 ≤ a(n) ≤ 4) so they fit in any seq_t type. */
    static const seq_t table[5] = {1, 0, 4, 0, 0};

    return table[n % 5u];
}
