/*
 * A213902  Number of integers of the form 6*k+1 and 6*k-1 between prime(n)
 *          and prime(n+1).
 * https://oeis.org/A213902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers of the form 6*k±1 that lie strictly *
 *            between prime(n) and prime(n+1). The function works for any
 *            seq_t and * idx_t, using a simple incremental prime generator
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_mod6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_mod6.h"

/* Static storage for generated primes.  primes[0] is the first prime (2). */
static uint64_t *primes = NULL;
static size_t     prime_cnt = 0;
static size_t     prime_cap = 0;

/* Ensure that at least the first `needed` primes are generated.  The
 * generation uses trial division against previously found primes. */
static void ensure_primes(idx_t needed)
{
    if (prime_cnt >= (size_t)needed)
        return;

    /* Grow the prime buffer if necessary. */
    if (prime_cap < (size_t)needed) {
        size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
        while (new_cap < (size_t)needed)
            new_cap *= 2;
        uint64_t *newp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!newp)
            abort();
        primes = newp;
        prime_cap = new_cap;
    }

    uint64_t candidate = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;

    while (prime_cnt < (size_t)needed) {
        bool isprime = true;
        for (idx_t i = 0; i < (idx_t)prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime)
            primes[prime_cnt++] = candidate;
        ++candidate;
    }
}

/* Main sequence function.  It is safe for arbitrary call orders. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* for safety, although a(0) is undefined */

    /* We need primes up to index n+1 (1‑based). */
    ensure_primes(n + 1);

    uint64_t low  = primes[n - 1];   /* prime(n) */
    uint64_t high = primes[n];       /* prime(n+1) */

    /* Interval of integers strictly between the two primes. */
    uint64_t L = low + 1;
    uint64_t R = high - 1;
    if (L > R)
        return 0;

    return count_mod6(L, R, 1) + count_mod6(L, R, 5);
}
