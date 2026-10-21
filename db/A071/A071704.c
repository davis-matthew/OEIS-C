/*
 * A071704  Number of ways to represent the n-th prime as arithmetic mean of
 *          three other odd primes.
 * https://oeis.org/A071704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 736, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = number of unordered triples of odd primes (excluding the n‑th
 *   prime itself) whose sum equals 3 times that prime.
 *
 *   Equivalently, for p = A000040(n):
 *     a(n) = |{p1 ≤ p2 ≤ p3 : p1,p2,p3 odd primes, p1+p2+p3 = 3p,
 *              p1,p2,p3 ≠ p}|.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"

/* ---------------  Sieve helpers --------------- */

static uint8_t *prime_flag = NULL;   /* prime_flag[i] == 1 if i is prime   */
static idx_t   sieve_limit = 0;      /* current sieve upper bound           */
static uint64_t *odd_primes = NULL;  /* list of odd primes ≤ sieve_limit    */
static size_t   odd_prime_count = 0; /* number of elements in odd_primes    */

/* Build/extend sieve and odd prime list up to 'limit' (inclusive). */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* free old data */
    free(prime_flag);
    free(odd_primes);

    /* allocate new flag array */
    prime_flag = (uint8_t *)malloc((size_t)(limit + 1));
    if (!prime_flag) {
        exit(EXIT_FAILURE);
    }
    for (idx_t i = 0; i <= limit; ++i) {
        prime_flag[i] = 1;
    }
    if (limit >= 0) prime_flag[0] = 0;
    if (limit >= 1) prime_flag[1] = 0;

    /* sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (prime_flag[p]) {
            for (idx_t m = p * p; m <= limit; m += p) {
                prime_flag[m] = 0;
            }
        }
    }

    /* count and store odd primes (exclude 2) */
    size_t cnt = 0;
    for (idx_t i = 3; i <= limit; i += 2) {
        if (prime_flag[i]) {
            ++cnt;
        }
    }

    odd_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!odd_primes) {
        exit(EXIT_FAILURE);
    }

    size_t pos = 0;
    for (idx_t i = 3; i <= limit; i += 2) {
        if (prime_flag[i]) {
            odd_primes[pos++] = (uint64_t)i;
        }
    }

    odd_prime_count = cnt;
    sieve_limit = limit;
}

/* ---------------  Main sequence function --------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* nth prime (1-indexed); may be 2 */
    uint64_t target_prime = (uint64_t)get_prime(n);
    idx_t limit = (idx_t)(3 * target_prime);

    /* prepare sieve and odd prime list up to 3p */
    ensure_sieve(limit);

    seq_t count = 0;
    /* iterate over unordered triples p1 ≤ p2 ≤ p3 */
    for (size_t i = 0; i < odd_prime_count; ++i) {
        uint64_t p1 = odd_primes[i];
        if (p1 == target_prime)   /* exclude target prime */
            continue;

        /* maximum allowed value for p2 to keep p3 ≥ p2 */
        uint64_t max_p2 = (3 * target_prime - p1) / 2;

        for (size_t j = i; j < odd_prime_count; ++j) {
            uint64_t p2 = odd_primes[j];
            if (p2 == target_prime)
                continue;

            if (p2 > max_p2)     /* p3 would become < p2 */
                break;

            uint64_t sum12 = p1 + p2;
            if (sum12 > limit)   /* further p2 will only increase sum12 */
                break;

            uint64_t p3 = limit - sum12;  /* required third prime */
            if (p3 < p2)          /* ordering violated for larger p2 as well */
                break;

            if (p3 == target_prime)
                continue;        /* skip triples containing the target prime */

            /* check primality of p3 */
            if (p3 <= limit && prime_flag[(size_t)p3]) {
                count++;
            }
        }
    }

    return count;
}
