/*
 * A081242  Left-to-right binary enumeration.
 * https://oeis.org/A081242
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse( (n with its leading 1 bit removed, bits mapped
 *            0→1, 1→2) )
 * Range:     u8 n <= 15, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_log2_idx(idx_t n) {
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp > 1) {   /* n >= 2 here, so tmp will eventually become 1 */
        tmp >>= 1;
        ++k;
    }
    return k;
}

seq_t a(idx_t n) {
    /* For n < 2 the sequence is undefined; return 0. */
    if (n < 2) return 0;

    idx_t k = floor_log2_idx(n);   /* number of bits after removing the leading 1 */
    seq_t result = 0;

    for (idx_t i = 0; i < k; ++i) {
        /* Access ith bit (starting from LSB), map 0->1, 1->2, and append */
        seq_t digit = ((n >> i) & 1ULL) + 1;
        result = result * 10 + digit;
    }

    return result;
}
