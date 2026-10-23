/*
 * A135029  Numbers k such that k and k^2 both have digit sum 10. Multiples of
 *          10 are omitted.
 * https://oeis.org/A135029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth integer k such that digit_sum(k)=10,
 *            digit_sum(k*k)=10 and k%10!=0. The function searches forward and
 *            memoises results
 * Range:     u8 none, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;     /* array of found terms            */
static size_t len   = 0;        /* number of terms stored          */
static size_t cap   = 0;        /* allocated capacity of cache     */
static seq_t next_k = 1;        /* next candidate to test          */

/* Ensure cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cap)
        return;
    size_t newcap = cap ? cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;
    seq_t *newmem = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newmem) {
        /* Out of memory – abort program. */
        abort();
    }
    if (cache) {
        memcpy(newmem, cache, len * sizeof(seq_t));
        free(cache);
    }
    cache = newmem;
    cap   = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 0, return 0 for safety */

    /* Grow cache if necessary to hold up to the requested index. */
    ensure_cache_cap((size_t)n);

    /* Generate terms until we have at least 'n' of them. */
    while (len < (size_t)n) {
        seq_t k = next_k;
        ++next_k;                           /* advance to next integer */

        /* Exclude multiples of 10. */
        if ((k % (seq_t)10) == 0)
            continue;

        /* Check digit sum of k. */
        if (digit_sum(k) != (seq_t)10)
            continue;

        /* Check digit sum of k^2. */
        seq_t sq = k * k;                    /* works for built-in and big-int seq_t */
        if (digit_sum(sq) != (seq_t)10)
            continue;

        /* Found a valid term. Append to cache. */
        if (len == cap)
            ensure_cache_cap(len + 1);
        cache[len++] = k;
    }

    return cache[n - 1];
}
