/*
 * A056129  Final nonzero digit of n-th primorial.
 * https://oeis.org/A056129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = prime(n)#/10 mod 10 for n > 2
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the last non‑zero digit of the n‑th primorial.
 * It handles the required base cases 0, 1 and 2 explicitly and
 * uses a small dynamic prime sieve to generate the first n primes.
 * All arithmetic is performed modulo 10, so the result always fits
 * into a single digit.  The code is valid C99/C11 and also compiles
 * as C++17.  The only non‑standard cast is the C‑style cast of
 * realloc, which is necessary for C++ compilers.
 */

#include <stdint.h>
#include <stdlib.h>

/* Global prime table, stored incrementally.  The table contains
 * the first s_prime_count primes in ascending order.
 */
static uint64_t *s_primes = NULL;
static uint64_t  s_prime_count = 0;
static uint64_t  s_last_checked = 1;   /* greatest integer tested for primality */

/* Ensure that at least n primes are stored (0‑based indices 0 … n‑1). */
static void
ensure_primes(idx_t n)
{
    if (s_prime_count >= n)
        return;                       /* already sufficient */

    /* First prime is 2 – initialise if empty. */
    if (s_prime_count == 0) {
        s_primes = (uint64_t *)calloc(1, sizeof(uint64_t));
        s_primes[0] = 2;
        s_prime_count = 1;
        s_last_checked = 2;
    }

    uint64_t cand = s_last_checked + 1;      /* next integer to test */
    while (s_prime_count < n) {
        /* Skip even numbers greater than 2. */
        if (cand % 2 == 0 && cand != 2) {
            ++cand;
            continue;
        }

        unsigned is_prime = 1;
        for (uint64_t i = 0; i < s_prime_count; ++i) {
            uint64_t p = s_primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            uint64_t *tmp = (uint64_t *)realloc(s_primes,
                                                 (s_prime_count + 1) * sizeof(uint64_t));
            s_primes = tmp;
            s_primes[s_prime_count] = cand;
            ++s_prime_count;
        }
        ++cand;
    }

    s_last_checked = cand - 1;
}

/* Main sequence function.  n is a uint64_t index passed through the
 * generic type idx_t.  The result is returned as seq_t. */
seq_t a(idx_t n)
{
    /* Base cases for the first three terms. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    /* Compute primes up to the n‑th one. */
    ensure_primes(n);

    /* Compute product of the first n primes, discarding 2 and 5,
     * modulo 10 to keep everything small. */
    seq_t prod = 1;
    for (idx_t i = 0; i < n; ++i) {
        uint64_t p = s_primes[i];
        if (p == 2 || p == 5)
            continue;
        prod = (prod * (seq_t)(p % 10)) % 10;
    }
    return prod;
}
