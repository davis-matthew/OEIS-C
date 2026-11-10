/*
 * A065264  Inverse permutation to A065263.
 * https://oeis.org/A065264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   inverse of A065263; a(1)=2, a(3)=1; if m's binary starts "10":
 *            a(m)=2m; if m starts "11" and is odd (m>3): a(m)=floor(m/2); if
 *            m starts "11" and is even: a(m)=2^(L-1) + (complement of the
 *            lower L-1 bits of m), L = bit length of m
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    if (m == 3) return (seq_t)1;
    if (m == 1) return (seq_t)2;

    int len = 0;
    seq_t t = m;
    while (t > 0) {
        len++;
        t >>= 1;
    }

    /* second most significant bit */
    seq_t second = (m >> (len - 2)) & 1;
    if (second == 0) {
        return m << 1;
    }
    if ((m & 1) == 1) {
        return m >> 1;
    }
    seq_t mask = ((seq_t)1 << (len - 1)) - 1;
    seq_t low = m & mask;
    seq_t comp = mask ^ low;
    return ((seq_t)1 << (len - 1)) | comp;
}
