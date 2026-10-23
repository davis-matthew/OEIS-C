/*
 * A100490  Odd numbers ending in {1,3,7,9} that are not primes.
 * https://oeis.org/A100490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generated on demand: odd numbers ending in {1,3,7,9} that are not primes
 * (including 1).  The sequence starts 1,9,21,27,33,39,49,51,... */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    idx_t count = 1;               /* already counted the term 1 */
    seq_t current = 3;             /* start from the next odd number */

    while (1) {
        /* Skip numbers ending in 5 */
        if ((current % 10) != 5) {
            uint64_t cur64 = (uint64_t)current;   /* safe for the values needed */
            /* If the number is composite (not prime) and > 1, count it */
            if (!is_prime(cur64)) {
                if (count == n) return current;
                count++;
            }
        }
        current += 2;   /* next odd number */
    }
}
