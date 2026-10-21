/*
 * A065090  Natural numbers which are not odd primes: composites plus 1 and 2.
 * https://oeis.org/A065090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A065090:
   a(n) = the n‑th positive integer that is NOT an odd prime
   (so 1, 2, 4, 6, 8, 9, 10, …).  The function is evaluated by
   a linear search with trial‑division primality tests. */

/* The code is valid both as C11 and C++17. */

#include <stdint.h>
#include <stdbool.h>

/* Trial‑division primality test for 64‑bit values. */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* Return the n‑th element (n ≥ 1).  idx_t is guaranteed to be
   unsigned (e.g., uint64_t), seq_t may be an unsigned integer
   type or an arbitrary‑precision integer class with a
   constructor from uint64_t. */
seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t candidate = 1;

    while (true) {
        /* Skip odd primes only (2 is included). */
        bool skip = (candidate > 2) &&
                    ((candidate & 1) != 0) &&
                    is_prime(candidate);

        if (!skip) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
