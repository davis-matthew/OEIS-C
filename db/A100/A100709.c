/*
 * A100709  Trajectory of 1001 under "3x+1" map.
 * https://oeis.org/A100709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1001; a(n+1) = (a(n) is even ? a(n)/2 : 3*a(n)+1)
 * Range:     u8 none, u16 n <= 142, u32 n <= 142, u64 n <= 142, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1001;
    for (idx_t i = 0; i < n; ++i) {
        if (x & 1) {
            x = x * 3 + 1;
        } else {
            x = x / 2;
        }
    }
    return x;
}
