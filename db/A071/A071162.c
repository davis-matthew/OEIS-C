/*
 * A071162  Simple rewriting of binary expansion of n resulting A014486-codes
 *          for rooted binary trees with height equal to number of internal
 *          vertices. (Binary trees where at each internal vertex at least the
 *          other child is leaf).
 * https://oeis.org/A071162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by recursively rewriting the binary expansion
 *            of n with the rules 0 -> 01 1X -> 1 (rewrite X) 0 where X is the
 *            suffix after the first 1. The resulting bit string is then
 *            interpreted as a binary integer
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 65535, u64 n <= 65535,
 *            u128 n <= 65535, big any n
 * Validated: standard, pass, 66 terms through n = 9153 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t encode_len(idx_t v, idx_t len)
{
    if (len == 0) return (seq_t)0;
    idx_t msb = (v >> (len - 1)) & 1U;
    idx_t sublen = len - 1;
    idx_t mask = (sublen == 0) ? 0 : ((1ULL << sublen) - 1);
    seq_t rest = encode_len(v & mask, sublen);
    if (msb == 0) {
        /* prepend 01 */
        return ((seq_t)1 << (2 * sublen)) | rest;
    } else {
        /* prepend 1 and append 0 */
        return ((seq_t)1 << (2 * len - 1)) | (rest << 1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* compute bit-length of n */
    idx_t len = 0;
    for (idx_t t = n; t; t >>= 1) len++;
    return encode_len(n, len);
}
