/*
 * A097120  Expansion of (1-x)^2/((1-x)^3-3x^4).
 * https://oeis.org/A097120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097120: a(n) = 3a(n-1) – 3a(n-2) + a(n-3) + 3a(n-4)
   with a(0)=a(1)=a(2)=a(3)=1.  Recurrence holds for n>=4. */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;    /* memoised values   */
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_last = 0;   /* largest index computed (inclusive) */

/* Allocate a new array of seq_t with enough capacity and copy old data.
   In C++ we construct new elements with default construction. */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n)
        return;

    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    if (new_cap <= (size_t)n)
        new_cap = (size_t)n + 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort because the sequence cannot be computed. */
        abort();
    }

    /* Copy existing elements. */
    for (size_t i = 0; i < cache_cap; ++i) {
        new_cache[i] = cache[i];
    }

    /* Initialise the newly allocated slots. */
#ifdef __cplusplus
    for (size_t i = cache_cap; i < new_cap; ++i) {
        new(&new_cache[i]) seq_t();
    }
#endif

    /* Free old buffer and update globals. */
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the first four terms of the sequence. */
static void init_base(void)
{
    if (cache_cap < 4) {
        ensure_capacity(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache_last = 3;
    }
}

/* Compute a(n) for any n >= 0, possibly out of order. */
seq_t a(idx_t n)
{
    init_base();

    if (n <= cache_last)
        return cache[(size_t)n];

    ensure_capacity(n);

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t t1 = cache[(size_t)(i - 1)];
        t1 = t1 * 3;
        seq_t t2 = cache[(size_t)(i - 2)];
        t2 = t2 * 3;
        seq_t t3 = cache[(size_t)(i - 3)];
        seq_t t4 = cache[(size_t)(i - 4)];
        t4 = t4 * 3;

        seq_t sum_pos = t1 + t3 + t4;      /* 3*a(i-1) + a(i-3) + 3*a(i-4) */
        cache[(size_t)i] = sum_pos - t2;   /* minus 3*a(i-2) */
    }
    cache_last = n;
    return cache[(size_t)n];
}
