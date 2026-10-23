/*
 * A129958  First differences of A129957.
 * https://oeis.org/A129958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = 12k^2 + 6k * a(2k-1) = 12k^2 – 6k + 2 * where k = ⌊n/2⌋
 *            for even n, and k = ⌊n/2⌋ + 1 for odd n
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t half = n >> 1;          /* floor(n/2) */
    if ((n & 1u) == 0u)           /* even n:  n = 2k */
    {
        seq_t k = (seq_t)half;
        return (seq_t)12 * k * k + (seq_t)6 * k;
    }
    else                           /* odd n: n = 2k-1, k = half+1 */
    {
        seq_t k = (seq_t)(half + 1);
        return (seq_t)12 * k * k - (seq_t)6 * k + (seq_t)2;
    }
}
