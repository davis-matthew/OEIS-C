/*
 * A224619  Number of (n+4)X6 0..2 matrices with each 5X5 subblock idempotent.
 * https://oeis.org/A224619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) - a(n-7) + a(n-8) -
 *            a(n-9) + a(n-10) + a(n-12) - a(n-13) for n>18 Initial values
 *            (for 1 ≤ n ≤ 18): 20465, 15354, 19596, 22081, 23453, 24252,
 *            31702, 43635, 58076, 73708, 89988, 113491, 148896, 198454,
 *            263524, 344683, 449230, 588930
 * Range:     u8 none, u16 n <= 9, u32 n <= 49, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   seq_from_str, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_realloc.h"

static const char *init_str[18] = {
    "20465", "15354", "19596", "22081", "23453",
    "24252", "31702", "43635", "58076", "73708",
    "89988", "113491", "148896", "198454", "263524",
    "344683", "449230", "588930"
};

static seq_t *cache = NULL;        /* cache[0] unused, indices 1..n valid */
static size_t cache_size = 0;      /* allocated size (elements) */
static idx_t  max_computed = 0;    /* largest n with cache[n] valid */

/* Ensure the cache can hold terms up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    /* Needed size (elements) must be at least n+1 to index n. */
    size_t need = (size_t)n + 1;
    if (need <= cache_size)
        return;

    /* Reallocate with seq_realloc, which works for all seq_t types. */
    seq_t *tmp = seq_realloc(cache, cache_size, need);
    if (!tmp)
        abort();                 /* allocation failed */

    /* Zero newly allocated entries (they will be overwritten). */
    seq_t zero = 0;
    for (size_t i = cache_size; i < need; ++i)
        tmp[i] = zero;

    cache = tmp;
    cache_size = need;
}

/* Populate the cache with the first 18 terms. */
static void init_cache(void)
{
    ensure_capacity(18);
    for (idx_t i = 1; i <= 18; ++i)
        cache[i] = seq_from_str(init_str[i - 1]);
    max_computed = 18;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    /* Initialise cache if needed. */
    if (!cache || max_computed < 18)
        init_cache();

    /* Return cached value if it is already computed. */
    if (n <= max_computed)
        return cache[n];

    /* Grow cache to hold up to n. */
    ensure_capacity(n);

    /* Compute terms from max_computed+1 up to n using the recurrence. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t val = cache[k - 1];
        val += cache[k - 1];          /* 2*a(k-1) */
        val -= cache[k - 2];          /* -a(k-2) */
        val += cache[k - 3];          /* +a(k-3) */
        val -= cache[k - 4];          /* -a(k-4) */
        val -= cache[k - 7];          /* -a(k-7) */
        val += cache[k - 8];          /* +a(k-8) */
        val -= cache[k - 9];          /* -a(k-9) */
        val += cache[k - 10];         /* +a(k-10) */
        val += cache[k - 12];         /* +a(k-12) */
        val -= cache[k - 13];         /* -a(k-13) */
        cache[k] = val;
    }
    max_computed = n;
    return cache[n];
}
