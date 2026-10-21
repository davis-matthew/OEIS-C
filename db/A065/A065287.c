/*
 * A065287  Infinite binary tree inspired permutation of N: 1 -> 3, 11ab..yz
 *          -> 11ab..yz1, 10ab..y0 -> 10ab..y, 10ab..y1 -> 11ab..y0.
 * https://oeis.org/A065287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the binary‑transformation rules of OEIS
 *            A065287: - a(1) = 3. - If n in binary starts with 11, then a(n)
 *            = (n << 1) | 1. - If n starts with 10 and ends with 0, then a(n)
 *            = n >> 1. - If n starts with 10 and ends with 1, then a(n) = (
 *            (n | (1 << (msb‑1))) & ~1 ) where msb is the i...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 0) return (seq_t)0;           /* not used in the offset */

    /* bit length of n (msb position + 1) */
    unsigned L = 0;
    idx_t t = n;
    while (t) { t >>= 1; L++; }             /* L >= 2 for n >= 2 */

    /* second most significant bit: at position L-2 */
    if (((n >> (L - 2)) & 1ULL) != 0ULL) {
        /* binary starts with 11 */
        uint64_t res = ((uint64_t)n << 1) | 1ULL;
        return (seq_t)res;
    } else {
        /* binary starts with 10 */
        if ((n & 1ULL) == 0ULL) {
            /* pattern 10…0 : drop trailing 0 */
            uint64_t res = n >> 1;
            return (seq_t)res;
        } else {
            /* pattern 10…1 : change leading 10 to 11 and clear last bit */
            uint64_t mask = ((uint64_t)1ULL << (L - 2));
            uint64_t res = ( (n | mask) & ~1ULL );
            return (seq_t)res;
        }
    }
}
