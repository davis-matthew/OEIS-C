/*
 * A125134  "Brazilian" numbers ("les nombres brésiliens" in French): numbers
 *          n such that there is a natural number b with 1 < b < n-1 such that
 *          the representation of n in base b has all equal digits.
 * https://oeis.org/A125134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A125134 – Brazilian numbers
   Computes the n‑th term by bruteforce search.
   The function is memoised so that any order of calls works. */

#include <stdbool.h>
#include <stdlib.h>

/* forward de‑claration of helper used by seq_t arithmetic */
static bool is_brazilian(seq_t num);

/* memoisation state */
static seq_t   *cache          = NULL; /* cached terms */
static size_t  cache_len      = 0;      /* number of cached terms */
static size_t  cache_cap      = 0;      /* allocated capacity */
static uint64_t last_checked   = 6;      /* last integer examined */

/* helper
   expand the cache array to at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = p;
    cache_cap = new_cap;
}

/* true iff num (>=7) is a Brazilian number */
static bool is_brazilian(seq_t num)
{
    /* smallest Brazilian number is 7 */
    if ((uint64_t)num < 7)
        return false;

    uint64_t N = (uint64_t)num;
    uint64_t max_base = N - 2;   /* b < n-1 */

    for (uint64_t b = 2; b <= max_base; ++b)
    {
        uint64_t tmp = N;
        unsigned first_digit = 0;
        bool first = true;
        bool ok = true;

        while (tmp > 0)
        {
            unsigned d = (unsigned)(tmp % b);
            if (first)
            {
                first_digit = d;
                first = false;
            }
            else if (d != first_digit)
            {
                ok = false;
                break;
            }
            tmp /= b;
        }

        if (!ok || first_digit == 0)   /* all digits equal and non‑zero */
            continue;
        return true;   /* a valid base found */
    }
    return false;      /* no suitable base */
}

/* required external function */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, return 0 */
        return 0;

    /* extend cache until we have at least n terms */
    while (cache_len < n)
    {
        ++last_checked;                    /* next candidate integer */

        if (is_brazilian((seq_t)last_checked))
        {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
