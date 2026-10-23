/*
 * A037408  Positive numbers having the same set of digits in base 2 and base
 *          3.
 * https://oeis.org/A037408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   all_ones_ternary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037408: positive numbers whose binary and ternary
 * representations use the same set of digits.
 *
 * A number is valid iff
 *   • its ternary representation uses only digits 0 and 1
 *   • its ternary representation is not a ternary repunit (all 1's)
 *   • its binary representation is not a binary repunit (all 1's)
 *     (the number 1 is the only allowed repunit).
 *
 * This is equivalent to generating numbers equal to
 *   val = Σ_{i∈S} 3^i                 (S ⊆ ℕ)
 * for all non‑empty subsets S, and skipping the patterns described
 * above.  The ordering by increasing Σ 3^i is the same as by the
 * binary index that selects S, so we can iterate b=1,2,3,… and
 * generate the sequence lazily.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/all_ones_ternary.h"

/* Cache for generated terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;  /* number of cached terms */
static size_t cache_cap  = 0;  /* current cache capacity */
static idx_t  last_b     = 0;  /* largest binary index processed */

/* Ensure the cache has space for at least min_cap elements. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : (min_cap < 8 ? 8 : min_cap);
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of A037408 (n≥1).  The sequence is generated
 * lazily and cached so that repeated calls are fast. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined; harmless */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Next binary index (b ≥ 1). */
        ++last_b;
        seq_t val = 0;          /* value expressed as sum of powers of 3 */
        seq_t pow = 1;          /* 3^i */
        idx_t t   = last_b;     /* local copy of b */

        while (t) {
            if (t & 1ULL)
                val += pow;             /* add 3^i if the bit is set */
            pow *= 3;                   /* next power of 3 */
            t >>= 1;                    /* move to next bit */
        }

        /* Skip binary repunits (all‑ones) except 1. */
        if (val > 1) {
            seq_t x = val + 1;
            if ((x & (x - 1)) == 0U)
                continue;               /* binary representation is repunit */
        }

        /* Skip ternary repunits (all 1's) except 1. */
        if (val > 1 && all_ones_ternary(val))
            continue;

        /* Store the valid term. */
        grow_cache(cache_len + 1);
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
