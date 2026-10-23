/*
 * A169817  n-th prime with both a prime number of 0's and a prime number of
 *          1's in binary representation minus n-th semiprime with both a
 *          prime number of 0's and a prime number of 1's in their binary
 *          representation.
 * https://oeis.org/A169817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A144214(n) – A178350(n) * A144214(n) : n‑th prime whose
 *            binary representation has a prime * number of 0’s and a prime
 *            number of 1’s. * A178350(n) : n‑th semiprime (product of two
 *            primes) whose binary * representation has a prime number of 0’s
 *            and a prime * number of 1’s
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   count_prime_factors, is_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"   /* bool is_prime_small(idx_t n); */
#include "../utils/is_prime.h"         /* bool is_prime(idx_t n); */
#include "../utils/count_prime_factors.h" /* seq_t count_prime_factors(seq_t n); */

#define MAX_CACHE 40000   /* enough for n up to about 10000 */

/*---| static storage |---------------------------------------------------*/
static seq_t primes_cache[MAX_CACHE];
static size_t primes_count = 0;
static uint64_t primes_next = 2;   /* next integer to test for prime */

static seq_t semiprimes_cache[MAX_CACHE];
static size_t semiprimes_count = 0;
static uint64_t semiprimes_next = 4; /* next integer to test for semiprime */

/*---| helper: check if binary representation has prime #0's and #1's |---*/
static bool has_prime_zero_one_bits(uint64_t x)
{
    if (x == 0) return false;
    int ones  = __builtin_popcountll(x);
    int bits  = 64 - __builtin_clzll(x);
    int zeros = bits - ones;
    return is_prime_small(ones) && is_prime_small(zeros);
}

/*---| generate primes with the property up to index n |-----------------*/
static void generate_primes_up_to(idx_t n)
{
    if (n <= primes_count) return;
    while (primes_count < n) {
        if (is_prime(primes_next) && has_prime_zero_one_bits(primes_next)) {
            primes_cache[primes_count++] = (seq_t)primes_next;
        }
        ++primes_next;
    }
}

/*---| generate semiprimes with the property up to index n |--------------*/
static void generate_semiprimes_up_to(idx_t n)
{
    if (n <= semiprimes_count) return;
    while (semiprimes_count < n) {
        if (count_prime_factors((seq_t)semiprimes_next) == 2 &&
            has_prime_zero_one_bits(semiprimes_next)) {
            semiprimes_cache[semiprimes_count++] = (seq_t)semiprimes_next;
        }
        ++semiprimes_next;
    }
}

/*---| public function |---------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1; value for n<1 irrelevant */
    if (n > MAX_CACHE) {
        /* for safety, increase the cache size if needed (unlikely for tests) */
        return 0;
    }
    generate_primes_up_to(n);
    generate_semiprimes_up_to(n);
    seq_t p = primes_cache[n-1];
    seq_t s = semiprimes_cache[n-1];
    return p - s;
}
