/*
 * A118510  Define sequence S_m by: initial term = m, reverse digits and add 1
 *          to get next term. It is conjectured that S_m always reaches a
 *          cycle of length 9, as in A117230. Sequence gives records for
 *          number of steps to reach cycle.
 * https://oeis.org/A118510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9*n - 8 if n is odd; a(n) = 9*n if n is even
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t result = (seq_t)9 * N;
    if (n & 1) {
        result -= (seq_t)8;
    }
    return result;
}
