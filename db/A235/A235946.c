/*
 * A235946  Number of (n+1) X (n+1) 0..2 arrays colored with the upper median
 *          value of each 2 X 2 subblock.
 * https://oeis.org/A235946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 41*a(n-4) - 400*a(n-8) (valid for n > 13) Initial values:
 *            a(1) = 81 a(2) = 722 a(3) = 1494 a(4) = 2356 a(5) = 5358 a(6) =
 *            10420 a(7) = 21288 a(8) = 49860 a(9) = 114312 a(10) = 228100
 *            a(11) = 449256 a(12) = 1104660 a(13) = 2526024
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 22, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;        /* 1‑based array of computed terms */
static idx_t cache_len = 0;        /* allocated length (>= n+1) */
static idx_t max_computed = 0;     /* highest index that has been filled */

/* initialise cache with the first 13 terms, all other positions zeroed */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache_len = 14;           /* indices 0..13, 0 unused */
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    if (!cache)
        return;               /* allocation failure – keep NULL */

    cache[0] = (seq_t)0;
    cache[1] = (seq_t)81;
    cache[2] = (seq_t)722;
    cache[3] = (seq_t)1494;
    cache[4] = (seq_t)2356;
    cache[5] = (seq_t)5358;
    cache[6] = (seq_t)10420;
    cache[7] = (seq_t)21288;
    cache[8] = (seq_t)49860;
    cache[9] = (seq_t)114312;
    cache[10] = (seq_t)228100;
    cache[11] = (seq_t)449256;
    cache[12] = (seq_t)1104660;
    cache[13] = (seq_t)2526024;

    max_computed = 13;
}

/* ensure that indices 0 .. n are valid in the cache array */
static void ensure_cache(idx_t n)
{
    init_cache();
    if (n + 1 <= cache_len)
        return;

    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        return;                       /* allocation failed – leave old data */

    cache = new_cache;
    cache_len = new_len;
    /* zero-initialize the newly added positions */
    for (idx_t i = max_computed + 1; i <= n; ++i)
        cache[i] = (seq_t)0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure_cache(n);

    if (n <= max_computed)
        return cache[n];

    /* compute intermediate terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 13) {
            /* values 1..13 were already provided */
            continue;
        }
        seq_t val = (seq_t)41 * cache[i - 4] - (seq_t)400 * cache[i - 8];
        cache[i] = val;
    }

    max_computed = n;

    return cache[n];
}
