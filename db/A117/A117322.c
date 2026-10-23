/*
 * A117322  a(n) = prime(n) modulo semiprime(n).
 * https://oeis.org/A117322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) modulo semiprime(n)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, _smallest_prime_factor, build_primes_upto,
 *            is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_smallest_prime_factor.h"

/* Test whether x is a product of exactly two primes (allowing repeats). */
static bool
is_semiprime(uint64_t x)
{
    if (x < 4) return false;        /* 4 is the smallest semiprime */
    uint64_t p = _smallest_prime_factor(x);
    if (p == x) return false;       /* prime numbers are not semiprimes */
    uint64_t q = x / p;
    return is_prime((idx_t)q);
}

/*
 * Return the nth term of OEIS sequence A117322.
 * n is 1‑based. The function works for any n >= 1.
 */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for non‑positive n */

    /* ---------- nth prime ---------- */
    idx_t prime_limit = _prime_limit_for_idx(n);
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(prime_limit, &primes, &prime_cnt);

    /* If the limit was insufficient, retry with larger limits. */
    while (prime_cnt < (size_t)n) {
        idx_t new_limit = prime_limit * 2;
        free(primes);
        build_primes_upto(new_limit, &primes, &prime_cnt);
        prime_limit = new_limit;
    }

    uint64_t nth_prime = primes[n - 1];
    free(primes);

    /* ---------- nth semiprime ---------- */
    size_t semi_cnt = 0;
    uint64_t semi_val = 0;
    for (uint64_t x = 4; ; ++x) {
        if (is_semiprime(x)) {
            ++semi_cnt;
            if (semi_cnt == (size_t)n) {
                semi_val = x;
                break;
            }
        }
    }

    /* ---------- result ---------- */
    return (seq_t)nth_prime % (seq_t)semi_val;
}
