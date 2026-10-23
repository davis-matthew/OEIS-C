/*
 * A100895  a(n) = smallest n-digit prime formed by appending a digit to
 *          a(n-1); a(1) = 7.
 * https://oeis.org/A100895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime_seq, resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 7;
   a(n) = smallest prime formed by appending one decimal digit to a(n‑1);
   The sequence stops once no such prime exists. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/resize_array.h"

/* Internal cache of computed terms: cache[0] = a(1), cache[1] = a(2), … */
static seq_t *cache = NULL;
static idx_t cached_n = 0;          /* number of terms stored (non‑zero) */

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for index 0 */
        return (seq_t)0;

    /* If we already have the term, return it directly. */
    if (n <= cached_n)
        return cache[n - 1];

    /* Grow the cache to hold up to term n. */
    cache = resize_array(cache, cached_n, n);
    if (cached_n == 0)          /* initialise the very first term */
    {
        cache[0] = (seq_t)7;
        cached_n = 1;
    }

    seq_t cur = cache[cached_n - 1];

    for (idx_t i = cached_n + 1; i <= n; ++i)
    {
        bool found = false;
        seq_t cand = 0;

        /* try appending digits 0…9 */
        for (int d = 0; d <= 9; ++d)
        {
            cand = cur * (seq_t)10 + (seq_t)d;
            if (is_prime_seq(cand))
            {
                found = true;
                break;
            }
        }

        if (!found)        /* no prime extension, sequence terminates */
        {
            cache[i - 1] = (seq_t)0;
            cached_n = i - 1;      /* last valid term */
            return (seq_t)0;       /* undefined for requested n */
        }

        cache[i - 1] = cand;
        cur = cand;
    }

    cached_n = n;            /* all terms up to n are stored now */
    return cache[n - 1];
}
