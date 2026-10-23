/*
 * A195063  Numbers n such that BCR(n) is less than n, where BCR =
 *          binary-complement-and-reverse = A036044.
 * https://oeis.org/A195063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that BCR(n) < n (BCR = A036044).
   This function returns the nth such even integer, with offset 1
   (a(1) = 6). */

#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

/* Compute BCR(n) = reverse of the bitwise complement of n,
   where the complement is taken over the minimal number of bits
   required to represent n. */
static seq_t BCR(seq_t n)
{
    idx_t k = bitlen_seq_t(n);
    if (k == 0) return 0;
    /* mask with k low bits set to 1 */
    seq_t mask = ((seq_t)1 << k) - 1;
    seq_t comp = (~n) & mask;          /* complement of n over k bits */
    seq_t rev  = 0;
    for (idx_t i = 0; i < k; ++i) {
        rev = (rev << 1) | ((comp >> i) & 1);
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0, return 0 */
    idx_t found = 0;
    seq_t x = 2;                   /* only even candidates are needed */

    while (found < n) {
        seq_t bcr = BCR(x);
        if (bcr < x) {
            ++found;
            if (found == n) {
                return x;
            }
        }
        x += 2;                    /* next even integer */
    }

    /* Should never reach here */
    return 0;
}
