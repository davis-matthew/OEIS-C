/*
 * A088423  a(n) is the number of primes in arithmetic progression starting
 *          with 11 and with d = 2n.
 * https://oeis.org/A088423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of consecutive primes in the arithmetic progression
 * 11, 11+2n, 11+4n, … until the first composite term is reached.
 * For n ≥ 1 this yields the sequence A088423. */
#include <stdbool.h>
#include "../utils/_is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 // offset is 1; below not defined
    uint64_t d = (uint64_t)n * 2;          // common difference 2n
    uint64_t term = 11;                    // first term
    uint64_t cnt = 0;                      // count of primes
    while (_is_prime(term)) {
        ++cnt;
        term += d;
    }
    return (seq_t)cnt;
}
