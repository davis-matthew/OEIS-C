/*
 * A065680  Number of primes <= prime(n) which begin with a 1.
 * https://oeis.org/A065680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   upper_prime_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of primes ≤ prime(n) that begin with digit 1.
   a(n) = #{ p_k : k ≤ n , first_digit(p_k) = 1 }   (OEIS A065680) */
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/upper_prime_bound.h"

/* Shared state between calls */
static idx_t *primes        = NULL;    /* list of primes */
static seq_t *prefix_counts = NULL;    /* cumulative counts of 1‑digit primes */
static size_t primes_cnt    = 0;        /* number of primes stored */
static size_t primes_cap    = 0;        /* allocated capacity of arrays */
static idx_t  current_limit = 0;       /* largest processed number */

/* Re‑compute primes up to at least the nth prime. */
static void ensure_sieve(idx_t n)
{
    if (n <= primes_cnt) return;          /* already have enough */

    idx_t limit = upper_prime_bound(n);

    /* Allocate boolean array for the sieve */
    bool *is_prime = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) return;                /* allocation failed, give up */
    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    /* Standard Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Resize arrays if needed */
    size_t new_cap = (size_t)(limit + 1);
    if (primes_cap < new_cap) {
        idx_t  *new_primes      = (idx_t*)realloc(primes, new_cap * sizeof(idx_t));
        seq_t *new_prefix      = (seq_t*)realloc(prefix_counts, new_cap * sizeof(seq_t));
        if (!new_primes || !new_prefix) { free(is_prime); return; }
        primes        = new_primes;
        prefix_counts = new_prefix;
        primes_cap    = new_cap;
    }

    /* Rebuild prime list and prefix counts from scratch up to limit */
    primes_cnt = 0;
    seq_t cumulative = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) {
            primes[primes_cnt] = i;
            /* first decimal digit */
            uint32_t p = (uint32_t)i;
            while (p >= 10) p /= 10;
            if (p == 1) ++cumulative;
            prefix_counts[primes_cnt] = cumulative;
            ++primes_cnt;
        }

    current_limit = limit;
    free(is_prime);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, but return 0 */
    ensure_sieve(n);
    /* primes are 0‑based; n-1 gives the nth prime */
    return prefix_counts[n - 1];
}
