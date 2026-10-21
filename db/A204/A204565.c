/*
 * A204565  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A204565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/3 + 3^n + (2*7^n)/3 Equivalently, a(1)=8, a(2)=42,
 *            a(3)=256 and for n≥4 a(n) = 11*a(n-1) - 31*a(n-2) + 21*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based array of computed terms */
static idx_t  cache_cap = 0;         /* current capacity of cache   */
static idx_t  cache_max = 0;         /* highest n which has been computed */

static void
grow_cache(idx_t new_cap)
{
    /* Double capacity until it exceeds new_cap */
    while (cache_cap <= new_cap) {
        size_t new_size = (cache_cap == 0) ? 4 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* defined only for n ≥ 1 */

    /* Ensure we have space for index n */
    if (cache_cap <= n) grow_cache(n);

    /* Initialise the first three terms if needed */
    if (cache_max < 3) {
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)42;
        cache[3] = (seq_t)256;
        cache_max = 3;
    }

    /* Compute terms up to n (if not already done) */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        /* a(i) = 11*v1 + 21*v3 - 31*v2  */
        seq_t tmp = (seq_t)11 * v1 + (seq_t)21 * v3;
        seq_t val = tmp - (seq_t)31 * v2;
        cache[i] = val;
    }
    cache_max = n;

    return cache[n];
}
