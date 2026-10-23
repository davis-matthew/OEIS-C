/*
 * A129074  Numbers k such that (k-9)/8 is prime.
 * https://oeis.org/A129074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129074: numbers k such that (k‑9)/8 is prime.
   a(n) = 8 · prime(n) + 9,   a(1) = 25 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Simple dynamic list of primes                                       */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;   /* stored primes, 1‑based indexing in the code below   */
static size_t   prime_count = 0;   /* how many primes are stored                         */
static size_t   prime_cap   = 0;   /* current capacity of the array                      */

/* Resize the primes array to hold at least `needed` elements.        */
static void ensure_prime_capacity(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) { abort(); }
    primes = tmp;
    prime_cap = new_cap;
}

/* Test whether x is prime using the primes already discovered.      */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure that at least the first `n` primes are stored.             */
static void ensure_primes_up_to(idx_t n)
{
    if (n <= 0) return;
    if (prime_count >= (size_t)n) return;

    /* If we have no primes yet, seed the list with 2 and start from 3. */
    if (prime_count == 0) {
        ensure_prime_capacity(1);
        primes[0] = 2;
        prime_count = 1;
    }

    uint64_t candidate = (prime_count == 1) ? 3 : primes[prime_count - 1] + 2;

    while (prime_count < (size_t)n) {
        if (is_prime_uint64(candidate)) {
            ensure_prime_capacity(prime_count + 1);
            primes[prime_count++] = candidate;
        }
        candidate += 2;   /* only odd candidates after 2 */
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1, undefined for n<1 */
    ensure_primes_up_to(n);
    return (seq_t)primes[n-1] * (seq_t)8 + (seq_t)9;
}
