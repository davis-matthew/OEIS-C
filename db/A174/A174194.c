/*
 * A174194  10n-7 and 10n-3 are both primes.
 * https://oeis.org/A174194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer k such that 10*k-7 and 10*k-3
 *            are both prime
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0)  /* offset is 1 – undefined for n==0 */
        return 0;

    idx_t k = 1;          /* candidate integer */
    idx_t count = 0;      /* how many suitable k have been found */

    for (;;) {
        /* Compute the two numbers to test for primality. */
        uint64_t p1 = 10ull * k - 7ull;
        uint64_t p2 = 10ull * k - 3ull;

        /* Both must be prime. */
        if (is_prime(p1) && is_prime(p2)) {
            ++count;
            if (count == n)
                return (seq_t)k;   /* cast to the arbitrary‑precision type */
        }

        ++k;
    }
}
