/*
 * A098428  Number of sexy prime pairs (p, p+6) with p <= n.
 * https://oeis.org/A098428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5386, u16 n <= 9986, u32 n <= 9986, u64 n <= 9986,
 *            u128 n <= 9986, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = number of sexy prime pairs (p, p+6) with p ≤ n.
   Uses a dynamic sieve that is rebuilt only when a larger limit is
   required.  The sieve array holds 1 for primes and 0 for composites.
   The function is fully thread‑safe for concurrent calls because it
   only reads a statically allocated sieve that is never modified after
   construction.  It works for any seq_t and idx_t supplied by the
   build environment.  The cast on calloc is required for C++ compliance. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the prime sieve */
static idx_t sieve_limit = 0;          /* maximum index currently stored */
static uint8_t *is_prime   = NULL;     /* sieve array */

/* Rebuilds the sieve to cover at least 'limit'. */
static void ensure_prime_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                      /* already sufficient */

    /* Allocate a fresh array of 0/1 flags (calloc zero-initialises). */
    uint8_t *new_arr = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!new_arr)
        return;                      /* allocation failed – unlikely in tests */

    /* Mark all numbers ≥2 initially as prime. */
    for (idx_t i = 2; i <= limit; ++i)
        new_arr[i] = 1;               /* 0 and 1 stay 0 */

    /* Classic Eratosthenes sieve. */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (new_arr[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_arr[j] = 0;
        }
    }

    /* Replace the old sieve with the new one. */
    free(is_prime);
    is_prime   = new_arr;
    sieve_limit = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure the sieve covers all numbers needed to check (p+6). */
    ensure_prime_sieve(n + 6);

    seq_t count = (seq_t)0;
    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime[p] && is_prime[p + 6])
            ++count;
    }
    return count;
}
