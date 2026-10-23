/*
 * A008334  Number of distinct primes dividing p-1, where p = n-th prime.
 * https://oeis.org/A008334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime divisors of (p_n - 1), where p_n
 *            is the n-th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n) {
    /* n is 1‑based; get_prime(n) returns p_n. */
    uint64_t prime = (uint64_t)get_prime(n);
    uint64_t target = prime - 1U;          /* p_n - 1 */
    int cnt = count_distinct_primes(target);
    return (seq_t)cnt;
}
