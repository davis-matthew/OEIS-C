/*
 * A236014  Number of (n+1)X(4+1) 0..2 arrays colored with the sum of the
 *          maximum and minimum values of each 2X2 subblock.
 * https://oeis.org/A236014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (OEIS A236014)
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 35, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const idx_t CACHE_LIMIT = 20000;      /* safe for n up to ~10000 */

static seq_t *cache = NULL;                 /* pointer to dynamically sized cache */
static size_t cache_capacity = 0;           /* allocated size */
static int cache_initialized = 0;

/* Ensure the cache has space for at least n+1 elements (1‑based).  */
static void ensure_cache(idx_t n)
{
    if (cache_capacity >= (size_t)(n + 1))
        return;

    size_t desired = n + 1;
    if (desired < CACHE_LIMIT + 1)
        desired = CACHE_LIMIT + 1;          /* grow at least to CACHE_LIMIT */

    seq_t *tmp = NULL;
    if (cache == NULL)
        tmp = (seq_t *)malloc(sizeof(seq_t) * desired);
    else
        tmp = (seq_t *)realloc(cache, sizeof(seq_t) * desired);

    if (tmp == NULL) {
        /* Allocation failure – aborting is acceptable for this build. */
        abort();
    }

    /* Zero the newly allocated part. */
    if (desired > cache_capacity)
        memset(tmp + cache_capacity, 0, sizeof(seq_t) * (desired - cache_capacity));

    cache = tmp;
    cache_capacity = desired;
}

/* Initialize base values (a(1) … a(16)).  */
static void init_cache(void)
{
    if (cache_initialized)
        return;

    ensure_cache(16);

    cache[1]  = (seq_t)4672ULL;
    cache[2]  = (seq_t)12544ULL;
    cache[3]  = (seq_t)34288ULL;
    cache[4]  = (seq_t)95848ULL;
    cache[5]  = (seq_t)267424ULL;
    cache[6]  = (seq_t)771564ULL;
    cache[7]  = (seq_t)2218212ULL;
    cache[8]  = (seq_t)6382872ULL;
    cache[9]  = (seq_t)18265360ULL;
    cache[10] = (seq_t)52177252ULL;
    cache[11] = (seq_t)149258712ULL;
    cache[12] = (seq_t)427853252ULL;
    cache[13] = (seq_t)1226410112ULL;
    cache[14] = (seq_t)3513633704ULL;
    cache[15] = (seq_t)10058786964ULL;
    cache[16] = (seq_t)28806493432ULL;

    cache_initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* out of OEIS range */

    ensure_cache(n);
    init_cache();

    /* Compute any missing terms up to n using the empirical recurrence. */
    for (idx_t i = 17; i <= n; ++i) {
        if (cache[i] != 0)
            continue;                 /* already known */

        seq_t pos = (seq_t)4   * cache[i - 2] +
                    (seq_t)3   * cache[i - 3] +
                    (seq_t)24  * cache[i - 4] +
                    (seq_t)21  * cache[i - 5] +
                    (seq_t)28  * cache[i - 6];

        seq_t neg = (seq_t)64  * cache[i - 7] +
                    (seq_t)199 * cache[i - 8] +
                    (seq_t)396 * cache[i - 9] +
                    (seq_t)578 * cache[i - 10] +
                    (seq_t)30  * cache[i - 11] +
                    (seq_t)136 * cache[i - 12] +
                    (seq_t)192 * cache[i - 13];

        cache[i] = pos - neg;
    }

    return cache[n];
}
