/*
 * A084889  Duplicate of A000404.
 * https://oeis.org/A084889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084889: numbers that can be expressed as the sum of two
   positive squares (both components ≥ 1).  Indexing starts at 1: a(1) = 2. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute ⌊√n⌋ for unsigned 64‑bit n, without using floating point.  */
static idx_t integer_sqrt(idx_t n)
{
    if (n < 2) return n;           /* √0 = 0, √1 = 1 */

    idx_t low  = 1;
    idx_t high = n >> 1;           /* for n>1, √n < n/2 */
    if (high < 1) high = 1;

    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        idx_t div = n / mid;      /* avoid mid*mid overflow */

        if (mid <= div)          /* mid² ≤ n */
        {
            if (mid + 1 > div)   /* (mid+1)² > n */
                return mid;
            low = mid + 1;
        }
        else
            high = mid - 1;
    }
    /* should never reach here */
    return low - 1;
}

/* Test whether v ≥ 2 can be written as x² + y² with x,y ≥ 1. */
static bool is_sum_two_pos_squares(idx_t v)
{
    if (v < 2) return false;

    idx_t limit = integer_sqrt(v - 1);  /* x² ≤ v-1 */

    for (idx_t x = 1; x <= limit; ++x)
    {
        idx_t prod = x * x;
        idx_t rem  = v - prod;
        if (rem == 0)               /* would require y = 0 */
            continue;

        idx_t y = integer_sqrt(rem);
        if (y > 0 && y * y == rem)
            return true;
    }
    return false;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* a(1) is the first term; behaviour for n==0 is undefined, return 0. */
    if (n == 0) return 0;

    /* Cache of already found terms.  */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 1;   /* last integer examined (not in cache) */

    /* Extend cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Find next integer that satisfies the test. */
        do
        {
            ++last_checked;
        } while (!is_sum_two_pos_squares(last_checked));

        /* Grow the cache if needed. */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)last_checked;
    }

    return cache[(size_t)n - 1];
}
