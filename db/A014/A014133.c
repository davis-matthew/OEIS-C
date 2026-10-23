/*
 * A014133  Sum of a square and a triangular number.
 * https://oeis.org/A014133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A014133: sum of a square and a triangular number
    a(n) = n-th smallest integer that can be expressed as
    x^2 + y(y+1)/2 (with x,y ≥ 0). */

/*  The function builds the required values on demand, caching all
    numbers found up to the current limit.  For each new request it
    doubles the search range until enough terms are available.  The
    implementation keeps the search space small enough to satisfy
    the 2‑second per call requirement for n up to about 10 000. */

#include <stdint.h>
#include <stdlib.h>

/*  Caches the sequence values in increasing order  */
static seq_t *cached = NULL;          /* array of cached sequence values */
static size_t cached_len = 0;         /* number of cached values          */
static size_t limit_current = 0;      /* maximum value considered so far */

/*  Rebuild the cache up to the supplied numerical limit   */
static void recompute_cache(size_t limit)
{
    size_t idx_limit = limit;
    seq_t limit_s = (seq_t)limit;

    /* Allocate a boolean array to mark attainable sums */
    uint8_t *mark = (uint8_t*)calloc(idx_limit + 1, sizeof(uint8_t));
    if (!mark) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    /* Enumerate all sums x^2 + y(y+1)/2 ≤ limit          */
    for (idx_t y = 0;; ++y) {
        seq_t t = ((seq_t)y * ((seq_t)y + 1)) / 2;
        if (t > limit_s)
            break;
        for (idx_t x = 0;; ++x) {
            seq_t sum = t + (seq_t)x * (seq_t)x;
            if (sum > limit_s)
                break;
            mark[(size_t)sum] = 1;
        }
    }

    /* Count how many values we have found                */
    size_t new_len = 0;
    for (size_t i = 0; i <= idx_limit; ++i) {
        if (mark[i])
            ++new_len;
    }

    /* Allocate space for the new cache and fill it        */
    seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (!new_cache) {
        perror("malloc");
        free(mark);
        exit(EXIT_FAILURE);
    }

    size_t pos = 0;
    for (size_t i = 0; i <= idx_limit; ++i) {
        if (mark[i]) {
            new_cache[pos++] = (seq_t)i;
        }
    }

    free(mark);
    free(cached);
    cached = new_cache;
    cached_len = new_len;
    limit_current = idx_limit;
}

/*  Return a(n) for n ≥ 1.  The function is safe to call in any
    order of n and memoizes its results.  For n == 0 a safe default
    (0) is returned, but the OEIS offset starts at 1.              */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* offset starts at 1 */

    /* Ensure we have at least n terms in the cached sequence. */
    while (cached_len < (size_t)n) {
        size_t new_limit = (limit_current == 0) ? 1000 : limit_current * 2;
        recompute_cache(new_limit);
    }

    return cached[(size_t)(n - 1)];
}
