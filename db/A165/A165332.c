/*
 * A165332  Duplicate of A031179.
 * https://oeis.org/A165332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   the terms of A165332 are the integers that eventually reach a
 *            fixed point under the iteration x → sum of cubes of the decimal
 *            digits of x. The fixed points are the numbers 0, 1, 153, 370,
 *            371, and 407
 * Range:     i8 n <= 4, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation builds the sequence on demand and caches all
    computed terms.  For each integer starting from 0 we test whether
    its iterates hit one of the fixed points or fall into a cycle that
    does not contain a fixed point.  The test uses a small visited
    array of size 20001, which is sufficient because after one
    application of the digit‑cube sum a number of at most 20 digits
    becomes at most 14580.  Since the iteration rapidly shrinks a
    number, all intermediate values lie below 20000 except possibly
    the starting integer, which is never marked as visited when it
    is large.  The cached sequence allows every call to a() to run
    in time proportional to the largest n that has been requested
    so far.  */

#include <stdlib.h>
#include <stdint.h>

static seq_t sum_of_cubes(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        seq_t d = x % 10;
        sum += d * d * d;
        x /= 10;
    }
    return sum;
}

static int reaches_fixed(seq_t start, int *vis, int stamp)
{
    seq_t x = start;
    while (1) {
        /* Check for fixed points. */
        if (x == 0 || x == 1 || x == 153 || x == 370 || x == 371 || x == 407)
            return 1;                      /* eventual fixed point */
        /* For values below the visited array size, detect a loop. */
        if (x <= 20000) {
            int idx = (int)x;
            if (vis[idx] == stamp)
                return 0;                  /* cycle without fixed point */
            vis[idx] = stamp;
        }
        x = sum_of_cubes(x);
    }
}

seq_t a(idx_t n)
{
    /* static cache holding the first n terms seen */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of cached terms */
    static uint64_t tested = 0;       /* next integer to test */
    static int  visited[20001] = {0};
    static int  stamp = 1;            /* unique stamp for each test */

    if (n == 0)
        return 0;                     /* undefined for offset 0, safe fallback */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        if (reaches_fixed((seq_t)tested, visited, stamp)) {
            /* Append the new term to the cache. */
            size_t new_len = cache_len + 1;
            seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!new_cache) abort(); /* allocation failed */
            cache = new_cache;
            cache[cache_len] = (seq_t)tested;
            cache_len = new_len;
            stamp++;                /* use a fresh stamp for next number */
        }
        tested++;                    /* move to the next candidate integer */
    }

    return cache[(size_t)(n - 1)];
}
