/*
 * A142401  Primes congruent to 11 mod 48.
 * https://oeis.org/A142401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 413, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 11 modulo 48 by sieving primes
   up to an estimated bound. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_array   = NULL;   /* primes up to current sieve limit */
static size_t   prime_cnt      = 0;       /* number of primes in prime_array      */
static size_t   ap_prime_cnt   = 0;       /* primes ≡ 11 (mod 48) in that array      */
static size_t   sieve_limit    = 500;     /* current upper bound for sieve          */
static bool     initialized    = false;

/* Ensure that there are at least n primes congruent to 11 mod 48
   available in *prime_array. */
static void
ensure_sieve(idx_t n)
{
    if (initialized && ap_prime_cnt >= (size_t)n)
        return;

    while (ap_prime_cnt < (size_t)n) {
        free(prime_array);
        build_primes_upto((idx_t)sieve_limit, &prime_array, &prime_cnt);

        ap_prime_cnt = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (prime_array[i] % 48 == 11)
                ++ap_prime_cnt;
        }

        if (ap_prime_cnt < (size_t)n)
            sieve_limit <<= 1;          /* double the limit */
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1 */

    ensure_sieve(n);

    idx_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (prime_array[i] % 48 == 11) {
            ++count;
            if (count == n)
                return (seq_t)prime_array[i];
        }
    }

    return 0;   /* should never be reached */
}
