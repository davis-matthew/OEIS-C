/*
 * A065159  Binary string self-substitutions: a(n) is obtained by substituting
 *          the binary expansion of n for each 1-bit in the binary expansion
 *          of n.
 * https://oeis.org/A065159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the integer obtained by replacing each ‘1’ bit of the
 *            binary representation of n by the whole binary string of n
 *            itself, while each ‘0’ bit is replaced by a single zero
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 62, u64 n <= 318,
 *            u128 n <= 3070, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return 0;

    /* Compute the number of bits in n (at least 1) */
    unsigned len = 0;
    uint64_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }
    if (len == 0)   /* for completeness; n==0 handled earlier */
        len = 1;

    seq_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        if ((n >> i) & 1ULL) {          /* bit is 1 → append n */
            res = (res << len) | (seq_t)n;
        } else {                        /* bit is 0 → append a single 0 */
            res <<= 1;
        }
    }
    return res;
}
