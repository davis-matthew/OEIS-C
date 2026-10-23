/*
 * A139550  a(n) = lcm(1..floor(n/2)).
 * https://oeis.org/A139550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1..floor(n/2))
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* m = floor(n/2) */
    idx_t m = n >> 1;
    if (m <= 1) return (seq_t)1;   /* lcm of empty set is defined as 1 */

    /* Cache of primes up to the largest m seen so far */
    static uint64_t *primes = NULL;      /* array of primes */
    static size_t   primes_cnt = 0;      /* number of primes */
    static idx_t    primes_limit = 0;    /* current upper limit of stored primes */

    /* If a larger m is requested, grow the prime table */
    if (m > primes_limit) {
        free(primes);                    /* discard old table */
        build_primes_upto(m, &primes, &primes_cnt);
        primes_limit = m;
    }

    /* Compute lcm(1..m) as the product of the highest powers of each prime <= m */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < primes_cnt && primes[i] <= m; ++i) {
        uint64_t p   = primes[i];
        uint64_t power = p;
        /* Find the largest power of p that does not exceed m */
        while (power <= m / p) {
            power *= p;
        }
        result *= (seq_t)power;
    }

    return result;
}
