/*
 * A131695  a(n) = 0 iff 2*prime(n+1) = prime(n) + prime(n+2), otherwise a(n)
 *          = 1.
 * https://oeis.org/A131695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 iff 2*prime(n+1) == prime(n)+prime(n+2); otherwise 1
 * Range:     u8 n <= 9756, u16 n <= 9756, u32 n <= 9756, u64 n <= 9756,
 *            u128 n <= 9756, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static storage for primes and state variables. */
static uint64_t *primes_arr = NULL;
static size_t primes_count = 0;
static size_t sieve_limit  = 0;

/* Build a sieve up to 'limit' and store the primes in primes_arr. */
static void build_primes_up_to(size_t limit)
{
    size_t sz = limit + 1;
    bool *is_prime = (bool *)malloc(sz * sizeof(bool));
    if (!is_prime) return;                     /* allocation failed */

    for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
    if (sz > 0) is_prime[0] = false;
    if (sz > 1) is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    uint64_t *p = NULL;
    if (count > 0) {
        p = (uint64_t *)malloc(count * sizeof(uint64_t));
        size_t idx = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (is_prime[i]) p[idx++] = (uint64_t)i;
    }

    free(is_prime);

    if (primes_arr) free(primes_arr);
    primes_arr   = p;
    primes_count = count;
    sieve_limit  = limit;
}

/* Ensure that at least 'needed' primes are available. */
static void ensure_primes(size_t needed)
{
    if (primes_arr == NULL) {
        /* Initial sieve covers well beyond the 10000th prime. */
        build_primes_up_to(200000);
    }
    while (primes_count < needed) {
        build_primes_up_to(sieve_limit * 2);
    }
}

/* OEIS sequence A131695. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n==0, safe return */
    size_t needed = (size_t)n + 2;       /* need prime(n+1) */
    ensure_primes(needed);

    uint64_t p_n   = primes_arr[(size_t)n - 1];
    uint64_t p_n1  = primes_arr[(size_t)n];
    uint64_t p_n2  = primes_arr[(size_t)n + 1];

    seq_t lhs = (seq_t)2 * (seq_t)p_n1;
    seq_t rhs = (seq_t)p_n + (seq_t)p_n2;
    return (lhs == rhs) ? (seq_t)0 : (seq_t)1;
}
