/*
 * A100760  Primes of the form 47n+5.
 * https://oeis.org/A100760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 143, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 47*n + 5 by a simple primality test. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;         // offset starts at 1, but guard just in case
    idx_t count = 0;              // how many primes of the form found
    idx_t k = 0;                  // current integer in the progression 47*k+5
    uint64_t candidate;           // candidate prime, fits in 64 bits for required n

    while (count < n) {
        candidate = 47ULL * k + 5ULL;
        if (is_prime(candidate)) {
            count++;
        }
        k++;
    }
    return (seq_t)candidate;      // candidate fits into seq_t for all tested n
}
