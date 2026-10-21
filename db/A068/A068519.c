/*
 * A068519  If n is prime then a(n) = n, else a(n) = prime(n).
 * https://oeis.org/A068519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is prime, otherwise a(n) = prime(n)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime number.  We pre‑generate primes up to a safe bound
   (200000) with a simple sieve and cache them.  For indices beyond the
   pre‑computed range the routine is able to extend the list on demand.  The
   primality test uses trial division by these cached primes.  The function
   a(idx_t n) can be called in any order because the prime list is
   lazily initialized once.  All arithmetic stays within 64‑bit or smaller
   registers; seq_t may be larger but is used only as a harmless cast.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool primes_inited = false;
static uint32_t primes[200000];
static size_t primes_count = 0;

/* Initialise the prime table with a sieve up to 200000.  */
static void init_primes(void)
{
    const size_t limit = 200000;
    bool *sieve = (bool *)malloc(limit + 1);
    if (!sieve) exit(1);
    memset(sieve, 0, limit + 1);

    primes_count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            primes[primes_count++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = true;
            }
        }
    }
    free(sieve);
    primes_inited = true;
}

/* Test whether n is prime using the cached primes. */
static bool is_prime_n(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_count &&
                        (uint64_t)primes[i] * (uint64_t)primes[i] <= n; ++i) {
        if (n % primes[i] == 0) return false;
    }
    return true;
}

/* Return the k‑th prime number (k ≥ 1).  The list is expanded if needed. */
static seq_t prime_at(idx_t k)
{
    if (k == 0) return (seq_t)0;
    if (!primes_inited) init_primes();

    /* If already available, use it. */
    if (k <= primes_count) return (seq_t)primes[k - 1];

    /* Extend the list until we have k primes.  This branch is not needed
       for n ≤ 20000 but makes the routine robust. */
    uint32_t cand = primes[primes_count - 1] + 1;
    while (primes_count < k) {
        bool ok = true;
        for (size_t i = 0; i < primes_count &&
                            (uint64_t)primes[i] * (uint64_t)primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) { ok = false; break; }
        }
        if (ok) {
            primes[primes_count++] = cand;
            if (primes_count == k) break;
        }
        ++cand;
    }
    return (seq_t)primes[k - 1];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* behaviour below offset is irrelevant */
    if (!primes_inited) init_primes();
    if (is_prime_n(n)) return (seq_t)n;    /* n itself is prime */
    return prime_at(n);                    /* otherwise the n‑th prime */
}
