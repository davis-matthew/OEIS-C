/*
 * A031451  Numbers whose base-2 representation has 4 fewer 0's than 1's.
 * https://oeis.org/A031451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose binary representation has 4 more 1's than 0's */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Find the n-th number with 4 more 1s than 0s in its binary representation. */
seq_t a(idx_t n) {
    idx_t count = 0;          /* count of qualifying numbers found so far */
    idx_t len   = 4;          /* current bit length (must be even) */

    for(;;) {
        /* first number of this length has a leading 1 */
        idx_t start = 1ULL << (len - 1);
        idx_t end   = (1ULL << len) - 1;

        for(idx_t x = start; x <= end; ++x) {
            /* number of 1 bits in x */
            unsigned int ones = __builtin_popcountll((unsigned long long)x);

            /* condition: 2*ones - len == 4  <=>  ones = (len+4)/2  */
            if ((idx_t)(2 * ones) == len + 4) {
                ++count;
                if (count == n) {
                    return (seq_t)x;   /* cast to return type */
                }
            }
        }
        len += 2;              /* next even bit length */
    }
}
