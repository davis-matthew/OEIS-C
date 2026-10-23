/*
 * A142333  Primes congruent to 41 mod 45.
 * https://oeis.org/A142333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 41 (mod 45) Implementation uses a
 *            cache of candidates of the form 45*k+41 and checks each with
 *            is_prime_small
 * Range:     u8 n <= 2, u16 n <= 268, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

#define CACHE_CAP 20000          /* enough for n up to about 10000 */

/* Cache of found primes.  Static size eliminates need for realloc,
   which is unsafe for arbitrary‑precision seq_t in C++.               */
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;      /* number of primes cached so far */
static idx_t next_k = 0;          /* next k to test in 45*k+41 */

/* Fill cache until it contains at least n primes. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n)
    {
        seq_t candidate = (seq_t)45 * (seq_t)next_k + 41;
        ++next_k;
        if (is_prime_small(candidate))
        {
            if (cache_len < CACHE_CAP)
                cache[cache_len++] = candidate;
            else
            {
                /* Should never happen for n ≤ 10000, but guard anyhow. */
                exit(EXIT_FAILURE);
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; behaviour for n < 1 is irrelevant. */
    if (n == 0)
        return 0;

    ensure_cache(n);
    return cache[n - 1];
}
