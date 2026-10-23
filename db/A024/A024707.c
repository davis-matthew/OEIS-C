/*
 * A024707  Positions of multiples of 5 in A024702.
 * https://oeis.org/A024707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that ((prime(k) mod 5) mod 3) = 1
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    if (n < 1)
        return 0;
    idx_t index = 1;          /* current prime position (prime(1)=2) */
    uint64_t p = 2;           /* first prime */
    idx_t matched = 0;        /* count of indices satisfying the condition */

    while (1) {
        /* Check the condition for the current prime */
        if ((p % 5 % 3) == 1) {
            ++matched;
            if (matched == n)
                return (seq_t)index;
        }
        /* Move to the next prime */
        p = next_prime(p);
        ++index;
    }
}
