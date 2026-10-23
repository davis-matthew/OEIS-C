/*
 * A178244  Number of distinct permutations of binary digits (0's and 1's) in
 *          n.
 * https://oeis.org/A178244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = binomial(bitlen(n), popcount(n))
 * Range:     u8 n <= 781, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation based on the OEIS description:
   the number of distinct binary permutations of the non‑leading‑zero
   digits of n. */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine the length of the binary representation (without leading zeros),
       treating 0 as having length 1 (the single digit "0"). */
    idx_t bitlen;
    if (n == 0) {
        bitlen = 1;
    } else {
        idx_t t = n;
        bitlen = 0;
        while (t) {
            bitlen++;
            t >>= 1;
        }
    }

    /* Count the number of 1‑bits in n. */
    idx_t ones = 0;
    {
        idx_t t = n;
        while (t) {
            ones++;
            t &= (t - 1);   /* removes the lowest set bit */
        }
    }

    /* Return the binomial coefficient C(bitlen, ones). */
    return binom(bitlen, ones);
}
