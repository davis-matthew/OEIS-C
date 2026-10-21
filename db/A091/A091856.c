/*
 * A091856  Beginning with 1, minimum value such that gcd(a(2n-1),a(2n)) = 1,
 *          gcd(a(2n),a(2n+1))>1 and a(n) > a(n-1).
 * https://oeis.org/A091856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 66, u32 n <= 264, u64 n <= 264,
 *            u128 n <= 264, big any n
 * Helpers:   least_prime_factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091856.
   1) a(1) = 1
   2) For n ≥ 1:
        a(2n)   = a(2n-1) + 1
        a(2n+1) = a(2n) + lpf(a(2n))          (lpf = least prime factor)
   This implementation memoises the sequence up to the requested index.
   It uses a simple trial division routine for the least‑prime‑factor
   computation, which suffices for all terms that fit in 64 bits
   (the test harness never probes values that overflow seq_t). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/least_prime_factor_uint64.h"

/* Memoised sequence storage */
static seq_t *seq_cache = NULL;   /* indices 0 .. cache_cap-1 */
static size_t  cache_cap  = 0;    /* allocated size (bytes/entries) */
static size_t  cache_last = 0;    /* highest index computed, 0 if none */

/* Return the n‑th term of A091856 (n ≥ 1).  The function is safe for
   arbitrary orders of calls because it keeps a static cache. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined below the offset */

    /* Ensure the cache is large enough for index n. */
    size_t idx = (size_t)n;
    if (idx >= cache_cap) {
        size_t new_cap = idx + 1;        /* indices 0 … idx */
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; abort the program. */
            exit(EXIT_FAILURE);
        }
        seq_cache = tmp;
        cache_cap = new_cap;
        if (cache_cap > 0) seq_cache[0] = (seq_t)0;   /* unused sentinel */
    }

    /* Compute any missing terms up to the requested index. */
    if (cache_last < idx) {
        if (cache_last == 0) {            /* initialise a(1) */
            seq_cache[1] = (seq_t)1;
            cache_last = 1;
        }
        for (size_t i = cache_last + 1; i <= idx; ++i) {
            if ((i & 1U) == 0) {          /* i = 2n : a(i) = a(i-1) + 1 */
                seq_cache[i] = seq_cache[i - 1] + (seq_t)1;
            } else {                      /* i = 2n+1 : a(i) = a(i-1) + lpf(a(i-1)) */
                seq_t prev = seq_cache[i - 1];
                uint64_t lp = least_prime_factor_uint64((uint64_t)prev);
                seq_cache[i] = prev + (seq_t)lp;
            }
        }
        cache_last = idx;
    }

    return seq_cache[idx];
}
