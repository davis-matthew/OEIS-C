/*
 * A183354  One quarter the number of nX2 1..4 arrays with no two neighbors of
 *          any element equal to each other.
 * https://oeis.org/A183354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4; a(n) = 9 * 4^(n-1) for n >= 2 (equivalently a(n) = 4 *
 *            a(n-1) for n >= 3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return (seq_t)4;

    seq_t val = (seq_t)9;
    for (idx_t i = 1; i < n; ++i)
        val *= (seq_t)4;
    return val;
}
