/*
 * A164459  Number of binary strings of length n with no substrings equal to
 *          0001 0011 or 1100.
 * https://oeis.org/A164459
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-4) + a(n-5) - a(n-6) + a(n-7) (H. P.
 *            Dale, recurrence valid for n ≥ 11 with initial values a(4)=13,
 *            a(5)=21, a(6)=35, a(7)=58, a(8)=96, a(9)=159, a(10)=264)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 85, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* memcpy */

static seq_t *cache      = NULL;   /* array of computed values */
static size_t cache_cap  = 0;      /* allocated capacity, in elements */
static idx_t  last_comp  = 0;      /* largest n for which cache[n] is valid */

/* Expand cache to hold at least indices up to 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need + 1 <= cache_cap)
        return;                          /* already big enough */

    size_t new_cap = ((size_t)need + 1) * 2;
    seq_t *new_cache;

#if defined(__cplusplus)
    /* C++: use new/delete to construct objects. */
    new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
#else
    /* C: use malloc/free and memcpy for POD types. */
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache != NULL && cache_cap > 0)
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
    free(cache);
#endif

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Initialise known terms from n = 4 to n = 10 (first seven terms). */
static void init_cache(void)
{
    if (cache_cap == 0) {
        ensure_capacity(10);                 /* indices 0 … 10 needed */

        cache[4]  = (seq_t)13;
        cache[5]  = (seq_t)21;
        cache[6]  = (seq_t)35;
        cache[7]  = (seq_t)58;
        cache[8]  = (seq_t)96;
        cache[9]  = (seq_t)159;
        cache[10] = (seq_t)264;

        last_comp = 10;
    }
}

/* Compute all terms up to and including n, using the recurrence. */
static void compute_up_to(idx_t n)
{
    if (n <= last_comp)
        return;                     /* already computed */

    if (cache_cap == 0)
        init_cache();                /* ensure base values exist */

    ensure_capacity(n);

    for (idx_t i = last_comp + 1; i <= n; ++i) {
        if (i <= 10)                 /* pre‑initialised in init_cache */
            continue;

        seq_t term = cache[i - 1] * 2;
        term -= cache[i - 4] * 2;
        term += cache[i - 5];
        term -= cache[i - 6];
        term += cache[i - 7];
        cache[i] = term;
    }

    last_comp = n;
}

/* Public API as required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;             /* undefined for n < 4 */

    compute_up_to(n);
    return cache[n];
}
