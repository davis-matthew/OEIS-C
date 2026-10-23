/*
 * A175467  Write n in binary. Place a 0 between every pair of adjacent 1's,
 *          and place a 1 between every pair of adjacent 0's. a(n) is the
 *          decimal equivalent of the result.
 * https://oeis.org/A175467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is obtained by writing n in binary and inserting the *
 *            complement of each pair of adjacent equal bits between them. *
 *            The resulting bit string is interpreted as a binary number. *
 *            Example: n = 6 (110₂) → 1 0 1 0₂ = 10₂ = 10₁₀
 * Range:     u8 n <= 30, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour for 0 is undefined,
                            but we return 0 to avoid UB */
        return 0;

    /* number of bits in n */
    idx_t len = 0;
    for (idx_t t = n; t; t >>= 1)
        ++len;

    seq_t res = 0;
    seq_t prev = 0;          /* previous bit as seq_t */

    for (idx_t i = 0; i < len; ++i) {
        idx_t bit = (n >> (len - 1 - i)) & 1ULL;   /* current bit (idx_t) */

        if (i == 0) {        /* first bit, just copy */
            res = (seq_t)bit;
        } else {
            if (bit == prev) {            /* same as previous → insert opposite bit */
                seq_t inserted = (seq_t)(1 - bit);
                res = (res << 1) | inserted;
            }
            res = (res << 1) | (seq_t)bit;
        }
        prev = bit;
    }
    return res;
}
