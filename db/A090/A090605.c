/*
 * A090605  Numbers m such that m-th prime is of the form 60*k+1.
 * https://oeis.org/A090605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index m of the n-th prime of the form 60*k+1 (search:
 *            iterate primes and count those congruent to 1 mod 60)
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) // undefined for the offset-1 sequence, return 0 as a safe value
        return 0;

    idx_t count = 0;   // how many suitable primes seen
    idx_t idx   = 0;   // current prime index (1‑based)
    uint64_t prev = 0; // prime before the next one to find

    while (true) {
        // fetch the next prime after 'prev' (prev starts at 0 → first prime is 2)
        uint64_t p = next_prime(prev);
        prev = p;
        idx++; // prime p has index idx

        // test the congruence condition
        if (p % 60 == 1) {
            count++;
            if (count == n)
                return (seq_t)idx; // found the desired index
        }
    }
}
