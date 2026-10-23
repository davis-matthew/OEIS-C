/*
 * A153047  Numbers n such that 2*n-15 is not a prime.
 * https://oeis.org/A153047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153047: numbers n such that 2*n-15 is composite (>1).  Computed by linear search. */

#include <stdbool.h>

/* Test if x is a prime number (trial division). */
static inline bool isPrime(idx_t x)
{
    if (x < 2)          return false;
    if (x == 2)         return true;
    if (x % 2 == 0)     return false;
    for (idx_t i = 3; i <= x / i; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;

    while (true) {
        /* Skip values where 2*n-15 <= 1 (i.e., n <= 8). */
        if (candidate >= 9) {
            idx_t val = candidate * 2 - 15;
            if (val > 1 && !isPrime(val)) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
