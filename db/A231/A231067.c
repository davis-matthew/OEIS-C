/*
 * A231067  Number of black square subarrays of (n+1) X (2+1) binary arrays
 *          with no element equal to a strict majority of its diagonal and
 *          antidiagonal neighbors, with upper left element zero.
 * https://oeis.org/A231067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) - 5*a(n-4) + 2*a(n-6) (empirical recurrence)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 67, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* base values for n = 1 .. 6 */
static const seq_t base_val[7] = {0, 1, 3, 4, 11, 15, 42};

static seq_t *cache = NULL;   /* 1‑based index: cache[n] holds a(n) */
static idx_t  capacity   = 0; /* allocated number of slots (>= max index + 1) */
static idx_t  max_comp   = 0; /* largest n for which cache[n] is valid */

/* ensure cache contains values up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (!cache) {
        /* first allocation: at least slots for n and for indices 1..6 */
        capacity = (n < 7) ? 7 : n + 1;
        cache = (seq_t *)malloc((size_t)capacity * sizeof(seq_t));
        if (!cache) return; /* allocation failure: caller will use 0 */
        for (idx_t i = 1; i <= 6 && i < capacity; ++i)
            cache[i] = base_val[i];
        max_comp = 6;
    }

    if (max_comp > n)
        return;  /* already computed */

    if (capacity <= n) {
        idx_t new_cap = capacity * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute new terms using the recurrence:
       a(i) = 5*(a(i-2)-a(i-4)) + 2*a(i-6) */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t diff = cache[i - 2] - cache[i - 4];
        cache[i] = (seq_t)(5 * diff) + (seq_t)(2 * cache[i - 6]);
    }
    max_comp = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; a(0) is undefined */
    ensure_cache(n);
    return cache[n];
}
