/*
 * A212575  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          2|w-x|=|x-y|+|y-z|.
 * https://oeis.org/A212575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A212575
 *
 * Recurrence:
 *   a(n) = 2*a(n-2) + 2*a(n-3) - a(n-4) - 4*a(n-5) - a(n-6)
 *          + 2*a(n-7) + 2*a(n-8) - a(n-10)
 * Initial values (n = 0 … 9):
 *   a(0)=0, a(1)=1, a(2)=4, a(3)=17, a(4)=42, a(5)=85,
 *   a(6)=142, a(7)=235, a(8)=346, a(9)=495
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static const seq_t BASE[10] = {
    0, 1, 4, 17, 42, 85, 142, 235, 346, 495
};

/* Memoization cache for n >= 10 */
static seq_t *cache = NULL;      /* array of computed values */
static size_t cache_capacity = 0;/* allocated length of cache  */
static idx_t cache_filled = 0;   /* highest index for which cache[i] is valid */

/* Ensure that cache holds values up to and including n */
static void ensure_cache(idx_t n)
{
    if (n < 10)
        return; /* values <10 are returned directly from BASE */

    /* Grow allocation if necessary */
    if ((size_t)(n + 1) > cache_capacity) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* Initialise base values if they are missing */
    if (cache_filled < 9) {
        for (idx_t i = cache_filled + 1; i <= 9; ++i)
            cache[i] = BASE[i];
    }

    /* Compute values from the next missing index up to n */
    idx_t start = cache_filled + 1;
    if (start < 10) start = 10;   /* recurrence starts at n = 10 */
    for (idx_t k = start; k <= n; ++k) {
        /* Positive terms */
        seq_t pos = cache[k-2] * 2;
        pos += cache[k-3] * 2;
        pos += cache[k-7] * 2;
        pos += cache[k-8] * 2;

        /* Negative terms */
        seq_t neg = cache[k-4];
        neg += cache[k-5] * 4;
        neg += cache[k-6];
        neg += cache[k-10];

        cache[k] = pos - neg;
    }

    cache_filled = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 10)
        return BASE[n];

    ensure_cache(n);
    return cache[n];
}
