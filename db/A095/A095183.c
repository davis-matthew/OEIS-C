/*
 * A095183  a(n) = least prime of the form prime(n+1) + k*prime(n).
 * https://oeis.org/A095183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the least prime of the form prime(n+1)+k*prime(n) with
 *            k>=1
 * Range:     u8 n <= 11, u16 n <= 352, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Prime generation (sieve of Eratosthenes up to a fixed limit).      */
/* ------------------------------------------------------------------ */

#define SIEVE_LIMIT 2000000   /* sufficient for n up to at least 10000 */

static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static bool     primes_ready  = false;

/* One‑time sieve initialization */
static void init_primes(void)
{
    if (primes_ready) return;

    uint8_t *sieve = (uint8_t*)calloc(SIEVE_LIMIT + 1, 1);
    if (!sieve) {
        /* allocation failure – fall back to empty list */
        return;
    }

    /* Sieve */
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                sieve[j] = 1;
            }
        }
    }

    /* Count primes first to allocate exact space */
    size_t cnt = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) ++cnt;
    }

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) {
        free(sieve);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) prime_list[idx++] = i;
    }
    prime_count = cnt;

    free(sieve);
    primes_ready = true;
}

/* Check primality of an unsigned integer using the precomputed primes */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    /* Trial division by primes up to sqrt(n) */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Sequence implementation                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* outside the offset – safety */

    /* Ensure the prime table is ready */
    init_primes();

    /* We need at least the (n+1)-th prime (1‑based index). The primes array
       is 0‑based: prime_list[0] == 2 == prime(1).             */
    if ((size_t)n >= prime_count) {
        /* This case should not occur for the intended use (n up to 10 000). */
        return (seq_t)0;
    }

    uint64_t p_prev = prime_list[n - 1]; /* prime(n)   */
    uint64_t p_next = prime_list[n];     /* prime(n+1) */

    /* Search for the smallest k >= 1 such that
       cand = p_next + k * p_prev is prime.               */
    uint64_t k = 1;
    while (true) {
        uint64_t candidate = p_next + k * p_prev;
        if (is_prime_uint64(candidate)) {
            return (seq_t)candidate;
        }
        ++k;
    }
}
