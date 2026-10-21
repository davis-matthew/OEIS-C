/*
 * A056651  Numbers k such that binomial(k,floor(k/2)) has no non-unitary
 *          square divisors: all of their square divisors are unitary ones.
 * https://oeis.org/A056651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 129, u32 n <= 129, u64 n <= 129,
 *            u128 n <= 129, big any n
 * Helpers:   exp_fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation finds the n‑th term of OEIS A056651.
   A k is included iff every exponent in the prime factorisation of
   binom(k, floor(k/2)) is at most 2.  The function tests each k
   sequentially, using a simple prime sieve and Legendre’s formula. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/exp_fact.h"

static const idx_t SIEVE_LIMIT = 200000;   /* safe upper bound for n ≤ 10000 */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* Build a list of primes up to SIEVE_LIMIT using a basic sieve. */
static void init_primes(void) {
    if (prime_list) return;
    bool *is_prime = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;
    prime_count = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            prime_count++;
    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint64_t)i;
    free(is_prime);
}

/* Returns true iff binom(k, floor(k/2)) has no prime exponent ≥ 3. */
static bool qualifies(idx_t k) {
    init_primes();                    /* ensure primes are ready */

    idx_t m = k >> 1;                 /* floor(k/2) */
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > k) break;             /* no need to test larger primes */

        idx_t e = exp_fact(k, p);
        if (k % 2 == 0) {             /* even k: subtract twice m! */
            e -= 2 * exp_fact(m, p);
        } else {                      /* odd k: subtract m! and (k-m)! */
            e -= exp_fact(m, p);
            e -= exp_fact(k - m, p);
        }
        if (e >= 3) return false;     /* violates the property */
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* the offset is 1, but safety */

    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        if (qualifies(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
