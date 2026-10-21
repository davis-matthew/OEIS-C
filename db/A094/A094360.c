/*
 * A094360  Pair reversal of Jacobsthal-Lucas numbers.
 * https://oeis.org/A094360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n+1)-1 for even n, a(n) = 2^(n-1)+1 for odd n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Decide parity */
    if ((n & 1) == 0) {
        /* Even n: a(n) = 2^(n+1) - 1 */
        seq_t power = ((seq_t)1) << (n + 1);
        return power - 1;
    } else {
        /* Odd n: a(n) = 2^(n-1) + 1 */
        seq_t power = ((seq_t)1) << (n - 1);
        return power + 1;
    }
}
