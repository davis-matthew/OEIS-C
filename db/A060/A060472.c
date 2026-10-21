/*
 * A060472  Smallest positive a(n) such that the number of solutions to
 *          a(n)=a(j)+a(k), j<k<n is four or less.
 * https://oeis.org/A060472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060472 – smallest positive a(n) such that the number of
 * solutions to a(n) = a(j) + a(k), j < k < n is at most four.
 *
 * The function generates the terms incrementally, keeping a static
 * cache of already computed values.  For each new term it starts
 * from the previous value plus one and scans upwards until a number
 * has at most four representations as the sum of two distinct
 * earlier terms.  The search stops as soon as the count exceeds
 * four, so the algorithm runs in O(n^2) time with a modest constant.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of already computed terms */
static seq_t *cache   = NULL;  /* array of terms */
static idx_t  cap     = 0;     /* current capacity of cache   */
static idx_t  sz      = 0;     /* number of terms stored      */

/* grow the cache to hold at least 'need' elements */
static void ensure_cap(idx_t need)
{
    if (need <= cap)
        return;
    idx_t newcap = (cap > 0) ? cap * 2 : 16;
    while (newcap < need)
        newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cap   = newcap;
}

/* count representations of value 'x' using the first (m-1) terms
 * (indices 0 .. m-2).  Returns the count (capped at 5 for speed). */
static idx_t count_reps(idx_t m, seq_t x)
{
    if (m <= 1)   /* no earlier terms */
        return 0;

    idx_t i = 0;
    idx_t j = m - 2;       /* last index of earlier terms */
    idx_t cnt = 0;

    while (i < j && cnt <= 4) {
        seq_t sum = cache[i] + cache[j];
        if (sum == x) {
            ++cnt;
            ++i;
            --j;
        } else if (sum < x) {
            ++i;
        } else {
            --j;
        }
    }
    return cnt;
}

/* public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0)                       /* offset is 1 – 0 is unused */
        return 0;

    ensure_cap(n);
    if (sz >= n)                      /* already computed */
        return cache[n-1];

    for (idx_t m = sz + 1; m <= n; ++m) {
        seq_t cand = (m == 1) ? (seq_t)1
                              : cache[m-2] + (seq_t)1;  /* > previous term */

        /* find the first candidate with at most 4 representations */
        while (true) {
            if (count_reps(m, cand) <= 4)
                break;
            ++cand;
        }
        cache[m-1] = cand;
    }
    sz = n;
    return cache[n-1];
}
