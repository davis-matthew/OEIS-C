/*
 * A108976  Numbers k such that 17*k + 19 is prime.
 * https://oeis.org/A108976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th non‑negative integer k such that 17*k + 19 is prime
   (offset 1).  The function scans k = 0,1,2,… and returns the first k
   that makes 17*k+19 prime for the requested n. */
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t    count = 0;          // how many primes seen so far
    uint64_t k     = 0;          // current candidate

    while (1) {
        uint64_t val = (uint64_t)17 * k + 19;
        if (is_prime(val)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
