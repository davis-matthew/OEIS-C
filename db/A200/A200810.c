/*
 * A200810  Iterate k -> d(k) until an odd prime is reached.
 * https://oeis.org/A200810
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the last odd prime obtained by iterating the divisor-count
 * function τ(k) (number of positive divisors) starting from k=n.
 * That is, replace k by τ(k) repeatedly until the result is an odd prime.
 * For odd primes n this returns n immediately.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t max_precomp = 0;
static uint16_t *tau_cache = NULL;   /* tau(k) for 0 <= k <= max_precomp */
static bool     *prime_flag = NULL;  /* true if 0 <= k <= max_precomp is prime */

/* Helper to recompute divisor counts and primes up to @n */
static void recompute_up_to(idx_t n)
{
    idx_t new_max = n;
    if (new_max == max_precomp) return;

    /* Resize delta arrays */
    if (tau_cache == NULL)
        tau_cache = (uint16_t *)calloc((size_t)(new_max + 1), sizeof(uint16_t));
    else
        tau_cache = (uint16_t *)realloc(tau_cache, (size_t)(new_max + 1) * sizeof(uint16_t));

    if (prime_flag == NULL)
        prime_flag = (bool *)calloc((size_t)(new_max + 1), sizeof(bool));
    else
        prime_flag = (bool *)realloc(prime_flag, (size_t)(new_max + 1) * sizeof(bool));

    /* Initialize counts to 0 and prime flags to true */
    for (idx_t i = 0; i <= new_max; ++i) {
        tau_cache[i] = 0;
        prime_flag[i] = (i >= 2);
    }

    /* Compute divisor counts */
    for (idx_t i = 1; i <= new_max; ++i) {
        for (idx_t j = i; j <= new_max; j += i)
            ++tau_cache[j];
    }

    /* Sieve of Eratosthenes for primes */
    for (idx_t i = 2; (idx_t)i * i <= new_max; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= new_max; j += i)
                prime_flag[j] = false;
        }
    }

    max_precomp = new_max;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0; /* undefined for n < 3 */

    /* Ensure precomputed data covers this n */
    if (n > max_precomp)
        recompute_up_to(n);

    /* If n itself is an odd prime, return it */
    if (prime_flag[n] && (n & 1))
        return (seq_t)n;

    /* Start iterating τ(k) */
    idx_t current = tau_cache[n];
    while (1) {
        /* If current is an odd prime, return it */
        if (prime_flag[current] && (current & 1))
            return (seq_t)current;

        /* Compute τ(current).  current is guaranteed <= max_precomp
           because τ of number ≤ number.  But to be safe, extend if needed */
        if (current > max_precomp) {
            recompute_up_to(current);
        }
        current = tau_cache[current];
    }
    /* Unreachable */
    return 0;
}
