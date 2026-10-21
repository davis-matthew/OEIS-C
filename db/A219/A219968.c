/*
 * A219968  Number of tilings of a 3 X n rectangle using straight (3 X 1)
 *          trominoes and 2 X 2 tiles.
 * https://oeis.org/A219968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 46, u64 n <= 90, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of tilings of a 3×n rectangle using (3×1) trominoes
 * and 2×2 squares (OEIS A219968).
 *
 * Recurrence (valid for n ≥ 9):
 *   a(n) = a(n-1) + 3·a(n-3) – 2·a(n-4) – a(n-6) + a(n-7) + a(n-9)
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=4,
 *   a(6)=8, a(7)=13, a(8)=19.
 *
 * The implementation memoises previously computed values so calls may
 * be made in arbitrary order.  All arithmetic uses unsigned seq_t
 * operations only, complying with the restrictions in the problem
 * statement.  The code is valid both as C11 and C++17. */

#include <stdlib.h>   /* malloc, realloc, exit */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;      /* memoised terms a(0), a(1), … */
static idx_t  cache_size = 0;    /* allocated array size (in elements) */
static idx_t  cache_max  = 0;    /* largest n that has been computed */

/* Ensure cache can hold indices 0 .. need (inclusive). */
static void ensure_cache_size(idx_t need)
{
    if (cache_size <= need) {
        idx_t new_size = cache_size ? cache_size << 1 : 16;
        while (new_size <= need) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* unrecoverable error */
        cache = tmp;
        cache_size = new_size;
    }
}

/* Initialise the first nine terms of the sequence. */
static void init_cache(void)
{
    if (!cache) {
        ensure_cache_size(8);          /* indices 0..8 */
        cache[0] = 1; cache[1] = 1; cache[2] = 1;
        cache[3] = 2; cache[4] = 3; cache[5] = 4;
        cache[6] = 8; cache[7] = 13; cache[8] = 19;
        cache_max = 8;
    }
}

seq_t a(idx_t n)
{
    init_cache();

    /* Return cached value if already known. */
    if (n <= cache_max)
        return cache[(size_t)n];

    /* Grow cache to include the requested index. */
    ensure_cache_size(n);

    /* Compute new terms up to n. */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        /* Positive part: a(k-1) + 3*a(k-3) + a(k-7) + a(k-9) */
        seq_t pos = cache[(size_t)(k-1)];
        pos += cache[(size_t)(k-3)] * (seq_t)3;
        pos += cache[(size_t)(k-7)];
        pos += cache[(size_t)(k-9)];

        /* Negative part: 2*a(k-4) + a(k-6) */
        seq_t neg = cache[(size_t)(k-4)] * (seq_t)2;
        neg += cache[(size_t)(k-6)];

        cache[(size_t)k] = pos - neg;
    }

    cache_max = n;
    return cache[(size_t)n];
}
