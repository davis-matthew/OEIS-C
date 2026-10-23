/*
 * A030095  Primes p such that 666p is palindromic.
 * https://oeis.org/A030095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   _floor_isqrt_u64, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation: the nth prime p such that 666 × p is a decimal palindrome. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, so n>=1 for valid calls */

    /* Prime sieve using stored primes for trial division. */
    size_t primes_cap = 1024;
    size_t primes_cnt = 0;
    uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) return 0; /* allocation failure */

    /* Start with the first prime */
    primes[primes_cnt++] = 2;

    uint64_t candidate = 3;
    uint64_t found = 0;

    for (;;) {
        /* Check primality by trial division with previously found primes. */
        uint64_t sq = _floor_isqrt_u64(candidate);
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > sq) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* Store new prime */
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) { free(primes); return 0; }
                primes = tmp;
            }
            primes[primes_cnt++] = candidate;

            /* Test 666 * candidate for being a palindrome. */
            seq_t product = (seq_t)666 * (seq_t)candidate;
            if (is_palindrome(product, 10)) {
                ++found;
                if (found == n) {
                    free(primes);
                    return (seq_t)candidate;
                }
            }
        }

        /* Next odd candidate */
        candidate += 2;
    }
}
