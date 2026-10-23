/*
 * A039605  Numbers whose base-12 representation has the same nonzero number
 *          of 1's and 3's.
 * https://oeis.org/A039605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 7804, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039605: numbers whose base‑12 representation has the same
   nonzero number of 1's and 3's (each at least once).  Search incrementally
   and cache results.  */
#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* cached terms, 0‑based */
static size_t cache_cap = 0;     /* capacity of cache */
static size_t cache_len = 0;     /* number of terms currently stored */
static seq_t next_candidate = 1; /* next integer to test */

/* Return the n‑th term (n ≥ 1).  For n == 0 we return 0 as a safety value. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* grow cache until it contains the desired index */
    while (cache_len < (size_t)n)
    {
        seq_t cand = next_candidate;

        /* find next number with equal nonzero counts of 1's and 3's */
        for (;; ++cand)
        {
            unsigned cnt1 = 0, cnt3 = 0;
            seq_t tmp = cand;

            /* examine digits of cand in base‑12 */
            while (tmp > (seq_t)0)
            {
                unsigned digit = (unsigned)(tmp % (seq_t)12);
                if (digit == 1)
                    ++cnt1;
                else if (digit == 3)
                    ++cnt3;
                tmp /= (seq_t)12;
            }

            /* accept iff counts are equal and nonzero */
            if (cnt1 > 0 && cnt1 == cnt3)
                break;
        }

        /* enlarge cache if needed */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        /* store the found term and prepare for next search */
        cache[cache_len++] = cand;
        next_candidate = cand + (seq_t)1;
    }

    return cache[(size_t)n - 1];
}
