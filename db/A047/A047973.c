/*
 * A047973  Distance of n-th prime to nearest cube.
 * https://oeis.org/A047973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = distance from the nth prime to the nearest integer cube
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, floor_cube_root, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/floor_cube_root.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, any value is acceptable */

    /* Cache of primes.  We grow it lazily so each value is computed only once. */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;   /* number of primes stored */
    static size_t prime_cap = 0;   /* capacity of the primes array */
    static uint64_t last_prime = 0;/* largest prime generated so far */

    /* Generate primes until we have at least n of them. */
    while (prime_cnt < n) {
        uint64_t candidate = last_prime + 1;
        /* Find the next prime (trivial trial division is sufficient for our limits). */
        while (!is_prime(candidate))
            ++candidate;

        /* Grow the array if necessary. */
        if (prime_cnt == prime_cap) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 8;
            /* Ensure we don't overflow the capacity in size_t just in case. */
            ensure_capacity(new_cap, &prime_cap, (void **)&primes, sizeof(uint64_t));
        }

        primes[prime_cnt++] = candidate;
        last_prime = candidate;
    }

    /* The nth prime (indexing from 1). */
    uint64_t p = primes[n - 1];
    seq_t sp = (seq_t)p;

    /* Floor cube root of the prime. */
    seq_t c = floor_cube_root(sp);

    /* Compute the two neighbouring cubes. */
    seq_t cube_low  = c * c * c;
    seq_t c_next    = c + 1;
    seq_t cube_high = c_next * c_next * c_next;

    /* Distances to the two cubes. */
    seq_t diff_low  = sp >= cube_low ? sp - cube_low : cube_low - sp;
    seq_t diff_high = cube_high >= sp ? cube_high - sp : sp - cube_high;

    /* Return the smaller distance. */
    return diff_low < diff_high ? diff_low : diff_high;
}
