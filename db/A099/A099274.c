/*
 * A099274  Sum of the prime(n) primes following prime(n).
 * https://oeis.org/A099274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the sum of the next prime(n) primes after the nth prime.
 *            That is, a(n) = Σ_{k=n+1}^{n+prime(n)} prime(k) (prime(n) is the
 *            nth prime). The implementation builds a dynamic prime table with
 *            trial division
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are supplied by the build (e.g. seq_t = uint64_t or
   an arbitrary‑precision integer; idx_t = uint64_t). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime cache ------------------------------------------------- */
static idx_t *prime_cache      = NULL;   /* array of primes, 0‑based   */
static size_t prime_count      = 0;      /* number of primes stored    */
static size_t prime_capacity   = 0;      /* allocated size             */
static idx_t  next_candidate   = 2;      /* next integer to test       */

/* Ensure that the cache holds at least 'need' primes (1‑based count). */
static void ensure_primes(size_t need)
{
    while (prime_count < need) {
        /* Resize the cache if needed */
        if (prime_count >= prime_capacity) {
            prime_capacity = prime_capacity ? prime_capacity * 2 : 64;
            prime_cache = (idx_t *)realloc(prime_cache,
                                            prime_capacity * sizeof(idx_t));
        }

        /* Find the next prime, starting from 'next_candidate' */
        while (true) {
            bool is_prime = true;
            /* Skip even numbers greater than 2 */
            if (next_candidate != 2 && (next_candidate & 1) == 0) {
                is_prime = false;
            } else {
                for (size_t i = 0; i < prime_count; ++i) {
                    idx_t p = prime_cache[i];
                    if ((seq_t)p * (seq_t)p > (seq_t)next_candidate)
                        break;
                    if (next_candidate % p == 0) {
                        is_prime = false;
                        break;
                    }
                }
            }
            if (is_prime)
                break;          /* next_candidate is prime */

            ++next_candidate;
        }

        /* Store the found prime */
        prime_cache[prime_count++] = next_candidate;
        ++next_candidate;
    }
}

/* ---------- Sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)                    /* offset is 1, undefined below */
        return 0;

    /* Ensure we have the nth prime */
    ensure_primes((size_t)n);
    seq_t p_n = prime_cache[(size_t)(n - 1)];   /* nth prime is at index n-1 */

    /* Ensure we have enough primes to perform the sum */
    ensure_primes((size_t)(n + (idx_t)p_n));

    /* Sum over the next p_n primes (indices n .. n+p_n-1 in 0‑based array) */
    seq_t sum = 0;
    for (idx_t i = n; i < n + (idx_t)p_n; ++i) {
        sum += prime_cache[(size_t)i];
    }
    return sum;
}
