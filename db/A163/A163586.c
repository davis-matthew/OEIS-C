/*
 * A163586  a(1) = 3. For n > 1, Ulam's spiral is started with a(n-1), and the
 *          primes p on the NE spoke are considered. a(n) is the minimal p
 *          that is the lesser of a twin prime pair.
 * https://oeis.org/A163586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 3; for n > 1 let p = a(n-1).  
   The values on the NE spoke are p + 4*k^2 – 2*k for k ≥ 1.  
   a(n) is the smallest such value that is the lesser of a twin‑prime pair. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;    /* Cached sequence values */
static size_t cache_cap = 0;   /* Allocated capacity of cache */
static size_t computed = 0;    /* Number of terms computed so far */

/* Ensure the cache can hold at least 'n' elements (1‑based index). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 2;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute terms up to index 'n' (1‑based). */
static void build_up_to(idx_t n)
{
    while (computed < (size_t)n) {
        size_t idx = computed + 1;                    /* 1‑based index to compute */
        if (idx == 1) {
            cache[0] = (seq_t)3;                      /* a(1) = 3 */
        } else {
            seq_t prev = cache[computed - 1];         /* a(n‑1) */
            seq_t val = 0;
            for (idx_t k = 1; ; ++k) {                /* loop over k ≥ 1 */
                seq_t wk = (seq_t)k;
                seq_t part = ((seq_t)4 * wk * wk) - ((seq_t)2 * wk);
                val = prev + part;
                if (is_prime_big(val) && is_prime_big(val + (seq_t)2))
                    break;
            }
            cache[computed] = val;                    /* a(n) */
        }
        ++computed;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                                    /* out of domain */
    ensure_cache(n);
    build_up_to(n);
    return cache[n - 1];
}
