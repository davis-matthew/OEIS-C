/*
 * A203929  Number of (n+1) X 4 0..5 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 61*a(n-1) - 315*a(n-2) + 50*a(n-3) Initial values: a(1) =
 *            47916 a(2) = 2543436 a(3) = 140120856 Recurrence holds for n >=
 *            4
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* 1-based array: cache[1] is a(1) */
static idx_t cache_size = 0;     /* number of elements currently stored (excluding index 0) */

/* Ensure cache can hold at least (n+1) slots (index 0 unused) */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_size) return;
    idx_t new_size = cache_size ? cache_size * 2 : 4;
    if (new_size < n + 1) new_size = n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_size));
    /* initialize newly allocated slots to zero */
    for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
    cache_size = new_size;
}

/* Compute sequence value for index n (n >= 1) */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, return 0 as safe fallback */

    /* Constants for initial terms */
    static const seq_t A1 = (seq_t)47916;
    static const seq_t A2 = (seq_t)2543436;
    static const seq_t A3 = (seq_t)140120856;

    /* Ensure the cache is large enough */
    ensure_capacity(n);

    /* Base cases */
    if (n == 1) { cache[1] = A1; return A1; }
    if (n == 2) { cache[2] = A2; return A2; }
    if (n == 3) { cache[3] = A3; return A3; }

    /* If already computed, return stored value */
    if (cache[n] != 0) return cache[n];

    /* Compute up to n iteratively */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] == 0) {
            cache[i] = (61 * cache[i-1]) - (315 * cache[i-2]) + (50 * cache[i-3]);
        }
    }
    return cache[n];
}
