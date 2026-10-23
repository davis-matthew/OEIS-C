/*
 * A008335  Number of distinct primes dividing p+1 as p runs through the
 *          primes.
 * https://oeis.org/A008335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001221(get_prime(n)+1)
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
    /* n is 1‑based.  get_prime(n) gives the nth prime. */
    uint64_t prime = (uint64_t)get_prime(n);
    uint64_t v = prime + 1;
    int cnt = count_distinct_primes(v);
    return (seq_t)cnt;
}
