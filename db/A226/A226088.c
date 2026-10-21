/*
 * A226088  a(n) is the number of the distinct quadrilaterals in a regular
 *          n-gon, which Q3 type are excluded.
 * https://oeis.org/A226088
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) satisfies the following recurrence (valid for n ≥ 3):
 *   Let d(n) = a(n) – a(n‑1) for n ≥ 4, with a(3)=0, a(4)=1, a(5)=1.
 *   For n ≥ 6 define the second difference
 *         s(n) = d(n) – d(n‑1)
 *   where
 *         s(n) =  2        if n == 6,
 *                =  3        if n is even and n ≠ 6,
 *                = –1        if n is odd, n ≡ 3 (mod 4),
 *                = –2        if n is odd, n ≡ 1 (mod 4).
 *   Then 
 *         d(n)  = d(n‑1) + s(n),
 *         a(n)  = a(n‑1) + d(n).
 *
 * The function below implements this recurrence with caching, so each
 * call is O(n) in the worst case but consecutive calls reuse computed
 * values.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache      = NULL;   /* cache of computed terms          */
static size_t cache_cap  = 0;      /* current cache capacity (indices) */
static idx_t  computed_upto = 0;   /* highest index fully computed     */

static void ensure_capacity(idx_t n)
{
    if (n >= cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed */
        cache = tmp;
        /* initialise new elements to 0 (safety) */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 3)                 /* undefined below the offset  */
        return 0;

    /* initialise cache on the first call */
    if (!cache)
    {
        size_t init_cap = n > 6 ? (size_t)n + 1 : 7;   /* at least indices 0..6 */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();   /* allocation failed */
        for (size_t i = 0; i < init_cap; ++i) cache[i] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache[5] = 1;
        computed_upto = 5;
        cache_cap = init_cap;
    }

    ensure_capacity(n);

    if (n <= computed_upto)
        return cache[n];

    /* compute missing values up to n using the recurrence */
    /* d is the first difference d(k) = a(k) – a(k‑1)   */
    uint64_t d = (uint64_t)cache[computed_upto] -
                 (uint64_t)cache[computed_upto - 1];

    for (idx_t i = computed_upto + 1; i <= n; ++i)
    {
        /* compute s(i) */
        int s_int;
        if ((i & 1ULL) == 0) /* even */
        {
            if (i == 6ULL) s_int = 2;
            else           s_int = 3;
        }
        else /* odd */
        {
            if ((i % 4ULL) == 1ULL)
                s_int = -2;
            else
                s_int = -1;
        }

        /* update d = d + s(i) */
        if (s_int < 0)
            d -= (uint64_t)(-s_int);
        else
            d += (uint64_t)s_int;

        /* a(i) = a(i‑1) + d */
        cache[i] = cache[i - 1] + (seq_t)d;
    }

    computed_upto = n;
    return cache[n];
}
