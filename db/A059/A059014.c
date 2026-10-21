/*
 * A059014  Numbers that have an even number of 0's and an odd number of 1's
 *          in binary expansion.
 * https://oeis.org/A059014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer whose binary representation
 *            has an odd length (number of bits) and an odd number of 1's.
 *            That is equivalent to numbers with even number of 0's and odd
 *            number of 1's
 * Range:     u8 n <= 43, u16 n <= 9233, u32 n <= 9233, u64 n <= 9233,
 *            u128 n <= 9233, big any n
 * Helpers:   qualifies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/qualifies.h"

/* Main function: returns the n‑th element of A059014. */
seq_t a(idx_t n)
{
    idx_t  cnt  = 0;
    seq_t  num  = (seq_t)1;

    while (1) {
        if (qualifies(num)) {
            ++cnt;
            if (cnt == n)
                return num;
        }
        ++num;                          /* next integer */
    }
}
