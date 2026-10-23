/*
 * A010060  Thue-Morse sequence: let A_k denote the first 2^k terms; then A_0
 *          = 0 and for k >= 0, A_{k+1} = A_k B_k, where B_k is obtained from
 *          A_k by interchanging 0's and 1's.
 * https://oeis.org/A010060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = (sum of binary digits of n) mod 2
 * Range:     u8 n <= 9760, u16 n <= 9760, u32 n <= 9760, u64 n <= 9760,
 *            u128 n <= 9760, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;          /* parity accumulator */
    idx_t x = n;
    while (x) {
        res ^= 1;           /* toggle parity for each set bit */
        x &= x - 1;         /* drop the lowest set bit */
    }
    return res;
}
