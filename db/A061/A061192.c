/*
 * A061192  a(1) = 1; a(n+1) = a(1) +(a(2) +(a(3) +...(a(n-1)
 *          +a(n)^2)^2...)^2)^2.
 * https://oeis.org/A061192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(1) + (a(2) + (a(3) + ... (a(n-1) + a(n)^2)^2 ... )^2
 *            )^2 with a(1)=1, a(2)=1, a(3)=2 (i.e. compute nested squares
 *            starting from a(n)^2 up to a(2), then add a(1))
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* ---------- cache handling ---------- */

#ifdef __cplusplus
static std::vector<seq_t> cache;           /* indices 0..n, 0 unused */
static void ensure_cache(idx_t n)
{
    if (cache.size() <= n)
        cache.resize(n + 1);
}
#else
static seq_t *cache = NULL;               /* raw array, indices 0..n, 0 unused */
static idx_t cache_cap = 0;              /* number of allocated slots */

static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    idx_t old_cap = cache_cap;
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                          /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
    /* initialise new elements to 0 (so we know they are uncomputed) */
    if (new_cap > old_cap)
        memset(&cache[old_cap], 0, (new_cap - old_cap) * sizeof(seq_t));
}
#endif

/* ---------- compute a(n) ---------- */

seq_t a(idx_t n)
{
    /* ensures we can address cache[n] */
    ensure_cache(n);

    /* use 1‑based indices; cache[0] unused */

    /* find first uncomputed index */
    idx_t start = 1;
    while (start <= n && cache[start] != (seq_t)0)
        ++start;

    /* compute missing values in order */
    for (idx_t i = start; i <= n; ++i)
    {
        if (i == 1)
        {
            cache[1] = (seq_t)1;
            continue;
        }
        if (i == 2)
        {
            cache[2] = (seq_t)1;
            continue;
        }

        /* build nested squares */
        seq_t val = cache[i - 1];
        val = val * val;                           /* square of a(i-1) */

        for (idx_t k = i - 2; k >= 2; --k)
        {
            seq_t tmp = cache[k] + val;
            val = tmp * tmp;                       /* (a(k)+val)^2 */
        }

        cache[i] = (seq_t)1 + val;                 /* add a(1)=1 */
    }

    return cache[n];
}
