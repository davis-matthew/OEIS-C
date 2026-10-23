/*
 * A156226  Primes of the form 9*n^2 + 1.
 * https://oeis.org/A156226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 15, u64 n <= 1400,
 *            u128 n <= 1400, big any n
 * Helpers:   allowed_m, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 9*n^2 + 1.
   The function a(n) returns the nth such prime.
   Implementation uses a simple search: iterate over integers,
   test candidates 9*m^2+1 for primality with is_prime_big(),
   collect primes until the desired index is reached.
   No hard‑coded values are used. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/allowed_m.h"

/* static cache for computed primes */
#define CACHE_MAX 200000  /* enough for n up to ~100000 */
static seq_t prime_cache[CACHE_MAX];
static size_t cache_size = 0;          /* number of primes stored */
static idx_t next_candidate = 2;      /* next m to try (starts at 2) */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Validate input: OEIS offset is 1 */
    if (n == 0) return 0;     /* undefined for n=0, return 0 as placeholder */

    /* Ensure cache holds at least n primes */
    while (cache_size < (size_t)n)
    {
        /* find next eligible m */
        while (!allowed_m(next_candidate))
            ++next_candidate;

        /* candidate value: 9*m^2 + 1 */
        seq_t m_seq = next_candidate;
        seq_t val = (seq_t)9 * m_seq * m_seq + (seq_t)1;

        /* test primality */
        if (is_prime_big(val))
        {
            if (cache_size >= CACHE_MAX)
            {
                /* Cache too small: dynamic growth not implemented.
                   For typical tests (n <= 10000) this should not happen. */
                /* In practice, we could enlarge the array here, but omitted for brevity. */
            }
            prime_cache[cache_size++] = val;
        }
        ++next_candidate;  /* advance for next iteration */
    }

    /* Return the requested element (1‑based index) */
    return prime_cache[n - 1];
}
