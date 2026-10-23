/*
 * A047554  Numbers that are congruent to {1, 2, 6, 7} mod 8.
 * https://oeis.org/A047554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n>=1, a(n) = 8*floor((n-1)/4) + offset, where offset =
 *            {1,2,6,7}[(n-1)%4]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* Undefined for n<1, return 0 */
    idx_t k = n - 1;
    idx_t block = k / 4;
    idx_t r = k % 4;
    seq_t offset;
    switch (r) {
        case 0: offset = 1; break;
        case 1: offset = 2; break;
        case 2: offset = 6; break;
        default: offset = 7; break;  /* r == 3 */
    }
    idx_t res_idx = 8 * block + (idx_t)offset;
    return (seq_t)res_idx;
}
