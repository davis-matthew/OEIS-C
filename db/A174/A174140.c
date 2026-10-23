/*
 * A174140  Numbers congruent to k mod 25, where 10 <= k <= 24.
 * https://oeis.org/A174140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10 + ((n-1) % 15) + 25 * ((n-1) / 15)
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based index; offset=1 */
    idx_t k = n - 1;                /* zero-based rank */
    idx_t r = k % 15;               /* remainder 0..14 */
    idx_t q = k / 15;               /* number of full 15-blocks */

    /* compute result in seq_t */
    seq_t res = (seq_t)(10 + r) + (seq_t)25 * (seq_t)q;
    return res;
}
