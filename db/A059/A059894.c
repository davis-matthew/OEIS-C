/*
 * A059894  Complement and reverse the order of all but the most significant
 *          bit in binary expansion of n. n = 1ab..yz -> 1ZY..BA = a(n), where
 *          A = 1-a, B = 1-b, ... .
 * https://oeis.org/A059894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = complement and reverse of all bits after the leading 1 in
 *            n's binary expansion
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Determine the bit‑length of n (n >= 1). */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        len++;
        tmp >>= 1;
    }

    /* Special case: n == 1 has only a single leading 1. */
    if (len == 1) {
        return (seq_t)1;
    }

    /* Bits after the leading 1. */
    seq_t rest = (seq_t)n - ((seq_t)1 << (len - 1));

    /* Invert those bits and keep only the lower (len‑1) bits. */
    seq_t mask = ((seq_t)1 << (len - 1)) - 1;
    seq_t inv = (~rest) & mask;

    /* Reconstruct the result: leading 1 followed by reversed inverted bits. */
    seq_t res = ((seq_t)1 << (len - 1));
    idx_t k = len - 2;  /* highest position for the reversed bits */
    for (idx_t i = 0; i < len - 1; ++i) {
        if (inv & ((seq_t)1 << i)) {
            res |= ((seq_t)1 << (k - i));
        }
    }

    return res;
}
