/*
 * A034000  One half of triple factorial numbers.
 * https://oeis.org/A034000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (3*n-1)*a(n-1),  a(1) = 1
 * (Equivalently, a(n) = product_{j=0}^{n-1} (3*j+2) / 2)
 * With offset 1: a(1) is the first term listed in the OEIS entry.
 * This implementation memoises computed values up to the largest
 * requested index to allow arbitrary call order and to ensure
 * O(n) overall time for many queries.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;      /* array of computed a(n) values */
static idx_t  cache_cap = 0;     /* capacity of the array */
static idx_t  last_computed = 0; /* number of values that have been computed (>=0) */

/* Ensure the cache has room for at least up_to entries. */
static void
ensure_cache(idx_t up_to)
{
    if (up_to <= cache_cap)
        return;
    /* Grow by at least 2x to amortise allocations. */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < up_to)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* Memory allocation failed – abort. */
        exit(1);
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute the n-th term of sequence A034000 with offset 1. */
seq_t a(idx_t n)
{
    if (n < 1)   /* the sequence is defined only for n >= 1 */
        return 0;

    ensure_cache(n);

    /* Initialise the first term if not already done. */
    if (last_computed == 0) {
        cache[0] = (seq_t)1;     /* a(1) = 1 */
        last_computed = 1;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = (3*i-1) * a(i-1) */
        const seq_t prev = cache[i - 2];      /* a(i-1) */
        const seq_t mult = (seq_t)(3 * i - 1);
        cache[i - 1] = prev * mult;
    }
    last_computed = n;

    return cache[n - 1];
}
