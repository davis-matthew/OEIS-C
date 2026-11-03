/*
 * A118320  Duplicate of A108918.
 * https://oeis.org/A118320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = base + perm(base_offset) where base = 2^⌊log₂(n)⌋,
 *            base_offset = n−base, and perm is defined recursively: P₀ = [0];
 *            For k>0, let len = 2^(k−1). Then P_k = concat( P_{k−1}[0…len−2],
 *            P_{k−1}+len, P_{k−1}[len−1] )
 * Range:     i8 n <= 48, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t perm_value(idx_t k, idx_t offset)
{
    if (k == 0) return 0;
    idx_t len = ((idx_t)1) << (k - 1);          /* block size / 2 */
    idx_t first_size = len - 1;                 /* size of the first part */

    if (offset < first_size)                  /* part 1 */
        return perm_value(k - 1, offset);
    else if (offset < first_size + len) {      /* part 2 */
        idx_t sub_off = offset - first_size;
        return perm_value(k - 1, sub_off) + (seq_t)len;
    } else                                     /* last element */
        return perm_value(k - 1, len - 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) is undefined for this sequence */
    
    idx_t val = 1, k = 0;
    while (val <= n >> 1) {        /* find largest power of two ≤ n */
        val <<= 1;
        ++k;
    }

    idx_t offset = n - val;        /* position inside the block */
    seq_t perm = perm_value(k, offset);
    return (seq_t)val + perm;
}
