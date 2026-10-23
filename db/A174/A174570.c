/*
 * A174570  Numbers k such that sum of squares of factorials of digits of k is
 *          a power of 2.
 * https://oeis.org/A174570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that * sum_{digits d of
 *            k} (d!)^2 is a power of 2. * Computed by incremental search with
 *            memoisation
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Pre‑compute (d!)^2 for digits 0–9 (stored in idx_t because sums
 * can be > seq_t).  idx_t is uint64_t. */
static idx_t fact2[10];
static bool fact2_init = false;
static void init_fact2(void)
{
    if (fact2_init) return;
    fact2_init = true;
    idx_t f = 1;
    fact2[0] = 1;                     /* 0! = 1 */
    for (int d = 1; d <= 9; ++d)
    {
        f *= (idx_t)d;                 /* factorial */
        fact2[d] = f * f;              /* square of factorial */
    }
}

/* Test whether x is a power of two (x>0). */
static bool is_power_of_two_idx(idx_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

/* Dynamic array to store discovered sequence elements (as idx_t). */
static idx_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Grow cache to hold at least ‘need’ elements. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap == 0 ? 2 : cache_cap * 2;
    while (new_cap < need) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
    if (!tmp) exit(1);                /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Next integer to test.  Stored as idx_t so it can exceed seq_t. */
static idx_t next_candidate = 1;

/* Return n-th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0; return 0 */

    init_fact2();

    /* Return cached value if already known. */
    if ((size_t)n <= cache_len)
        return (seq_t)cache[n - 1];

    /* Ensure cache can store the required number of elements. */
    grow_cache((size_t)n);

    /* Generate terms until we have n of them. */
    while (cache_len < (size_t)n)
    {
        idx_t k = next_candidate;
        idx_t sum = 0;
        idx_t t = k;

        if (t == 0)
            sum += fact2[0];
        else
        {
            while (t > 0)
            {
                unsigned d = (unsigned)(t % 10);
                sum += fact2[d];
                t /= 10;
            }
        }

        if (is_power_of_two_idx(sum))
            cache[cache_len++] = k;

        ++next_candidate;
    }

    return (seq_t)cache[n - 1];
}
