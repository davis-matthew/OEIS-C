/*
 * A094747  Rearrangement of odd primes so that absolute successive
 *          differences are the least distinct even numbers. Priority is given
 *          to the least successive difference that has not occurred earlier.
 * https://oeis.org/A094747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   ensure_used_diff_cap, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A094747 by iterative construction.
   The algorithm follows the definition: starting from a(1)=3, each step
   chooses the smallest unused even difference 2*d (d=1,2,3,…) and tests
   a(n)-2*d (if it stays ≥3) and a(n)+2*d in that order for primality
   and for not being already in the list.  The first valid candidate is
   added to the sequence and the difference d is marked used.  The
   function is memoised; calls may be in any order up to about n=10000. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_used_diff_cap.h"

/* --------------------------------------------------------------------- */
/* Upper bound on the size needed for n≈10000.  This covers the required
   range and avoids dynamic reallocation which is problematic for
   arbitrary-precision seq_t types. */
#define MAX_CAP 200000

/* Static storage for the sequence and the used‑difference set. */
static seq_t  cache[MAX_CAP];
static size_t cache_len = 0;

static bool used_diff[MAX_CAP];

/* --------------------------------------------------------------------- */
/* Check whether `val` occurs in the first `upto` positions of `cache`. */
static bool contains(const seq_t *arr, seq_t val, size_t upto)
{
    for (size_t i = 0; i < upto; ++i)
        if (arr[i] == val) return true;
    return false;
}

/* --------------------------------------------------------------------- */
/* Ensure that the cache has room for at least `need` elements.  We keep
   it at a fixed maximum capacity; if the caller requests more we give
   an out‑of‑range value (0). */
static inline void ensure_cache_cap(size_t need)
{
    (void)need; /* unused, cache is fixed size */
    /* Above, cache_len and cache_cap are maintained automatically. */
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1 */

    /* Lazy initialisation. */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)3;  /* a(1) = 3 */
        cache_len = 1;
        /* used_diff[0] is unused; zero is default. */
    }

    /* Compute forward as needed. */
    while (cache_len < (size_t)n)
    {
        seq_t curr = cache[cache_len - 1];

        /* Search for the smallest unused d. */
        for (size_t d = 1; ; ++d)
        {
            if (d >= MAX_CAP) return (seq_t)0; /* safety guard */

            if (used_diff[d]) continue;

            /* Minus branch (when possible). */
            if (curr > (seq_t)(2 * d))
            {
                seq_t cand = curr - (seq_t)(2 * d);
                if (cand >= (seq_t)3 &&
                    is_prime_big(cand) &&
                    !contains(cache, cand, cache_len))
                {
                    used_diff[d] = true;
                    cache[cache_len++] = cand;
                    break;
                }
            }

            /* Plus branch. */
            seq_t cand = curr + (seq_t)(2 * d);
            if (is_prime_big(cand) &&
                !contains(cache, cand, cache_len))
            {
                used_diff[d] = true;
                cache[cache_len++] = cand;
                break;
            }
        }
    }
    return cache[n - 1];
}
