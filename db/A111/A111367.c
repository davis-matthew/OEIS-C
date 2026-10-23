/*
 * A111367  Numbers k such that 7*k + 5 is prime.
 * https://oeis.org/A111367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth non‑negative integer k such that 7*k + 5 is prime
 * Range:     u8 n <= 49, u16 n <= 6411, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined below the offset – behaviour does not matter */
        return (seq_t)0;

    uint64_t k       = 0;   /* candidate k */
    uint64_t count   = 0;   /* how many valid k have been found */

    for (;; ++k) {
        uint64_t val = 7 * k + 5;
        if (is_prime(val)) {
            ++count;
            if (count == n)
                return (seq_t)k;   /* cast to seq_t (unsigned or big integer) */
        }
    }
}
