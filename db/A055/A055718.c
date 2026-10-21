/*
 * A055718  Erroneous version of A056171.
 * https://oeis.org/A055718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = π(n) – π(⌊n/2⌋), except a(3)=1 (erroneous version)
 * Range:     i8 n <= 98, i16 n <= 98, i32 n <= 98, i64 n <= 98, i128 n <= 98,
 *            big any n
 * Helpers:   build_primes_upto, primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/primes_upto.h"

seq_t a(idx_t n)
{
    /* Special case: erroneous value for n = 3 */
    if (n == 3) return (seq_t)1;

    /* Static cache for primes */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t sieve_limit = 0;

    /* Extend sieve if needed */
    if (n > sieve_limit) {
        uint64_t *new_primes;
        size_t new_cnt;
        build_primes_upto(n, &new_primes, &new_cnt);
        free(primes);
        primes = new_primes;
        prime_cnt = new_cnt;
        sieve_limit = n;
    }

    size_t pi_n    = primes_upto((size_t)n, primes, prime_cnt);
    size_t half = (size_t)(n >> 1);          /* floor(n/2) */
    size_t pi_half = primes_upto(half, primes, prime_cnt);

    seq_t res = (seq_t)(pi_n - pi_half);
    return res;
}
