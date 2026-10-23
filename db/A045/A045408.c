/*
 * A045408  Primes congruent to {1, 3, 4, 5} mod 7.
 * https://oeis.org/A045408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes congruent to 1, 3, 4 or 5 modulo 7.
   The function builds an incremental prime list and caches
   the qualifying primes so that calls in any order are
   efficient.  For n > 0 it returns the n‑th such prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* The provided helper(s) – only the standard is_prime is used. */
extern bool is_prime(idx_t);

/* Utility: grow an array of seq_t values */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = *cap == 0 ? 8 : (*cap * 2);
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Utility: grow an array of uint64_t primes for trial division */
static void grow_primes(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = *cap == 0 ? 8 : (*cap * 2);
        if (new_cap < need) new_cap = need;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Test primality by trial division using a list of known primes. */
static bool is_prime_trial(uint64_t x, uint64_t *primes, size_t primes_len)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= x; ++i) {
        if (x % primes[i] == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached primes that satisfy the residue condition */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    static uint64_t *prime_list = NULL;   /* primes for trial division */
    static size_t primes_cap = 0;
    static size_t primes_len = 0;

    static uint64_t next_candidate = 2;    /* next integer to test for primality */

    if (n == 0) return (seq_t)0;   /* undefined for n < 1, return 0 */

    /* Extend cache until we have at least n qualifying primes. */
    while (cache_len < n) {
        /* Find next prime. */
        uint64_t m = next_candidate;
        next_candidate++;

        /* Primality test using dynamic list. */
        if (!is_prime_trial(m, prime_list, primes_len)) {
            /* Not prime – skip. */
            continue;
        }

        /* m is prime: store it */
        grow_primes(&prime_list, &primes_cap, primes_len + 1);
        prime_list[primes_len++] = m;

        /* Accept if prime ≡ 1,3,4,5 (mod 7). */
        switch (m % 7ULL) {
            case 1:
            case 3:
            case 4:
            case 5:
                grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = (seq_t)m;
                break;
            default:
                /* Not a desired residue; discard. */
                break;
        }
    }

    return cache[n - 1];
}
