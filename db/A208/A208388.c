/*
 * A208388  Number of n X 4 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 76*a(n-1) - 36*a(n-2) for n > 3; a(1) = 14, a(2) = 1017,
 *            a(3) = 76806
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* cache[0] unused, cache[1] = a(1), ... */
static size_t cache_size = 0;  /* number of allocated slots (0 .. cache_size-1) */
static idx_t max_computed = 0; /* largest n for which cache[n] is valid */

/* Ensure that cache contains terms up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Allocate initial space if needed */
    if (cache == NULL) {
        size_t init_size = (n + 1 > 4 ? (size_t)(n + 1) : 4);
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (cache == NULL)
            exit(EXIT_FAILURE);
        cache_size = init_size;
    }

    /* Resize if the table is too small */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialize base cases if necessary */
    if (max_computed < 1) {
        cache[1] = (seq_t)14;
        max_computed = 1;
    }
    if (max_computed < 2) {
        cache[2] = (seq_t)1017;
        max_computed = 2;
    }
    if (max_computed < 3) {
        cache[3] = (seq_t)76806;
        max_computed = 3;
    }

    /* Compute remaining terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        cache[k] = 76 * cache[k - 1] - 36 * cache[k - 2];
    }
    max_computed = n;
}

/* Main entry function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1; return 0 for safety */
        return 0;
    ensure_cache(n);
    return cache[n];
}
