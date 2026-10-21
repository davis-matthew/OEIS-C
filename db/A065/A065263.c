/*
 * A065263  Infinite binary tree inspired permutation of N: 1 -> 3, 11ab..yz
 *          -> 11ab..yz1, 10ab..y0 -> 10ab..y, 10ab..y1 -> 11AB..Y0 (where
 *          1AB..Y0 is the complement of 0ab..y1).
 * https://oeis.org/A065263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Permutation A065263: binary tree–inspired reorder.
    For n >= 1, let n be written in binary as 1 b_{len-2} … b_1 b_0.
    The value a(n) is computed as follows:

    1.  if n == 1                 -> a(n) = 3          (11₂)
    2.  if second bit b_{len-2} == 1
            a(n) = (n << 1) | 1   // append '1' at the end

    3.  otherwise (second bit is 0)
        3a. if last bit b_0 == 0
                a(n) = n >> 1        // drop last zero
        3b. if last bit b_0 == 1
                let m = n & ((1 << (len-1)) - 1)      // lower len-1 bits
                let comp = (~m) & ((1 << (len-1)) - 1) // complement of those bits
                a(n) = (1 << (len-1)) | comp
        (the result has the same number of bits as n)
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)3;
    }

    /* compute bit length of n (len >= 2) */
    int len = 0;
    idx_t tmp = n;
    while (tmp) {
        len++;
        tmp >>= 1;
    }

    /* second bit (bit at position len-2) */
    idx_t second = (n >> (len - 2)) & 1ULL;

    if (second) {               /* rule 2: append a 1 */
        return ((seq_t)n << 1) | 1;
    }

    /* second bit is 0: look at last bit */
    idx_t last = n & 1ULL;

    if (!last) {                /* rule 3a: drop trailing 0 */
        return (seq_t)(n >> 1);
    }

    /* rule 3b: complement lower len-1 bits and prepend a 1 */
    idx_t mask = ((idx_t)1 << (len - 1)) - 1;   /* mask for len-1 bits */
    idx_t m   = n & mask;                       /* lower len-1 bits of n */
    idx_t comp = (~m) & mask;                   /* bitwise complement */

    seq_t result = ((seq_t)1 << (len - 1)) | comp;
    return result;
}
