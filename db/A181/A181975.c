/*
 * A181975  The sequence {1, 2, 3, 4, 5, 4, 3, 2} repeated.
 * https://oeis.org/A181975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(mod((n+3),8)-4)+1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n+3) mod 8 using bitwise AND (safe for uint64_t). */
    idx_t m = (n + 3) & 7;
    int diff = (int)m - 4;
    if (diff < 0)
        diff = -diff;
    return (seq_t)(diff + 1);
}
