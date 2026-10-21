/*
 * A205737  Number of 2 X (n+1) 0..2 arrays with every 2 X 2 subblock having
 *          the same number of clockwise edge increases as its horizontal
 *          neighbors and no 2 X 2 subblock having the same number of
 *          counterclockwise edge increases as its vertical neighbors.
 * https://oeis.org/A205737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -18*a(n-2) -15*a(n-3) +36*a(n-4) -20*a(n-5)
 *            +11*a(n-6) -2*a(n-7) for n ≥ 8, with initial values a(1)=81,
 *            a(2)=369, a(3)=1764, a(4)=8655, a(5)=43155, a(6)=218034,
 *            a(7)=1112664 The implementation uses a memoised
 *            linear–recurrence evaluator
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* memoisation array, indexed by 0..n */
static size_t   cap    = 0;      /* allocated capacity of cache */
static idx_t    last   = 0;      /* highest n that has been computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n = 0, but guard anyway */
    /* initialise storage on first call */
    if (!cache) {
        cap = 8;                   /* at least hold indices 0..7 */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) return 0;      /* out of memory – silently return 0 */
        /* set the known first seven values */
        cache[1] = (seq_t)81;
        cache[2] = (seq_t)369;
        cache[3] = (seq_t)1764;
        cache[4] = (seq_t)8655;
        cache[5] = (seq_t)43155;
        cache[6] = (seq_t)218034;
        cache[7] = (seq_t)1112664;
        last = 7;
    }
    /* grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failure – fall back to 0 */
        /* zero the newly allocated part */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }
    /* return if already computed */
    if (n <= last) return cache[n];
    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence is valid for i ≥ 8 */
        if (i >= 8) {
            cache[i] = 9*cache[i-1]
                       - 18*cache[i-2]
                       - 15*cache[i-3]
                       + 36*cache[i-4]
                       - 20*cache[i-5]
                       + 11*cache[i-6]
                       - 2*cache[i-7];
        }
    }
    last = n;
    return cache[n];
}
