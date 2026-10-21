/*
 * A059013  Odd number of 0's and even number of 1's in binary expansion.
 * https://oeis.org/A059013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059013:  Odd number of 0's and even number of 1's in binary expansion
 *
 *  a(n) is the n-th non‑negative integer whose binary representation
 *  (without leading zeros) contains an odd number of zeroes and an even
 *  number of ones.  The first terms are:
 *      a(1)=0, a(2)=5, a(3)=6, a(4)=17, ...
 *
 *  The routine simply scans the integers in increasing order, counting
 *  those that satisfy the parity condition, and returns the n‑th one.
 *  It therefore works for any seq_t/idx_t pair provided by the build.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t   count = 0;          /* how many valid numbers found so far */
    uint64_t value = 0;         /* candidate integer, processed as uint64_t */

    while (true)
    {
        /* Count ones using popcount; available as a gcc/clang builtin. */
        unsigned ones = __builtin_popcountll(value);

        /* Length of the binary representation:
         *  - for value==0 we treat length as 1 ("0")
         *  - otherwise the number of bits is 64 - clz. */
        unsigned len = (value == 0) ? 1
                                    : 64 - __builtin_clzll(value);

        unsigned zeros = len - ones;          /* zeros are the remaining bits */

        /* Property: zeros odd AND ones even. */
        if ((zeros & 1U) && !(ones & 1U))
        {
            count++;
            if (count == n)
                return (seq_t)value;          /* cast to requested type */
        }

        value++;                               /* move to the next candidate */
    }
}
