/*
 * A142207  Primes congruent to 10 mod 41.
 * https://oeis.org/A142207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142207: primes congruent to 10 modulo 41.
   Computes the nth such prime (n ≥ 1).  The function is
   memoised and works for arbitrary builds (uint8_t … __int128,
   or arbitrary‑precision seq_t).  It uses trial division via
   is_prime_small (fast for numbers ≤ a few millions). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static seq_t *cache = NULL;      /* cached primes */
static size_t cap = 0;           /* allocated capacity */
static idx_t len = 0;            /* number of cached primes */
static idx_t next_k = 0;         /* multiplier for 41*k + 10 */

/* enlarge the cache
   (uses realloc; aborts on allocation failure) */
static void grow_cache(void)
{
    size_t new_cap = (cap == 0) ? 64 : cap << 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed */
    cache = tmp;
    cap = new_cap;
}

/* append a prime to the cache */
static void cache_append(seq_t p)
{
    if (len >= cap) grow_cache();
    cache[len++] = p;
}

/* find the next prime of the form 41*k+10, starting from
   the current value of next_k.  next_k is advanced past the
   found prime. */
static seq_t find_next_prime(void)
{
    while (true) {
        unsigned long long cand = 41ULL * (unsigned long long)next_k + 10ULL;
        ++next_k;                          /* advance for next call */
        if (cand < 2ULL) continue;          /* 10, 51, … are < 2 */
        if (is_prime_small((uint64_t)cand))
            return (seq_t)cand;
    }
}

/* return the nth term (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 1 */

    /* initialise the cache on first entry */
    if (!cache) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        len = 0;
    }

    while (len < n) {                         /* generate more primes */
        seq_t p = find_next_prime();
        cache_append(p);
    }
    return cache[n - 1];
}
