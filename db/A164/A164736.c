/*
 * A164736  Number of n-digit cycles of length 5 under the Kaprekar map
 *          A151949.
 * https://oeis.org/A164736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-3) + a(n-4) + a(n-5) - 2*a(n-6) +
 *            a(n-7) + a(n-8) - a(n-9) for n > 15 with initial values: a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=0, a(7)=0, a(8)=0, a(9)=0,
 *            a(10)=0, a(11)=1, a(12)=0, a(13)=3, a(14)=0, a(15)=5
 * Range:     u8 n <= 140, u16 n <= 140, u32 n <= 140, u64 n <= 140,
 *            u128 n <= 140, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* array of a(k) for k=1..size */
static size_t cache_capacity = 0;    /* allocated storage */
static idx_t cache_size = 0;         /* last computed index (n) */

/* Initial values for 1 ≤ n ≤ 15 */
static const seq_t base_vals[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 5 };

/* Grow the cache to hold at least 'needed' elements (needed ≥ 1) */
static void ensure_cache(idx_t needed)
{
    if (needed <= (idx_t)cache_capacity)
        return;

    size_t new_cap = cache_capacity ? cache_capacity << 1 : 1;
    while (new_cap < (size_t)needed)
        new_cap <<= 1;

    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();               /* allocation failure */

    /* zero the new part to keep values defined (not strictly needed) */
    if (new_cap > cache_capacity)
        for (size_t i = cache_capacity; i < new_cap; ++i)
            new_arr[i] = (seq_t)0;

    cache = new_arr;
    cache_capacity = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined for indices < 1 */

    if (n <= cache_size)
        return cache[n - 1];    /* already computed */

    /* Extend cache to hold at least n entries */
    ensure_cache(n);

    /* Compute terms from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 15) {
            cache[i - 1] = base_vals[i - 1];
        } else {
            /* Use recurrence with 0‑based array indices:
                 a(i) = a(i-1)+a(i-2)-2*a(i-3)+a(i-4)+a(i-5)
                        -2*a(i-6)+a(i-7)+a(i-8)-a(i-9)
               indices in cache: a(k) -> cache[k-1] */
            seq_t pos = cache[i - 2]          /* a(i-1) */
                      + cache[i - 3]          /* a(i-2) */
                      + cache[i - 5]          /* a(i-4) */
                      + cache[i - 6]          /* a(i-5) */
                      + cache[i - 8]          /* a(i-7) */
                      + cache[i - 9];         /* a(i-8) */

            seq_t neg = cache[i - 4];          /* a(i-3) */
            neg += cache[i - 4];              /* 2*a(i-3) */
            neg += cache[i - 7];              /* a(i-6) */
            neg += cache[i - 7];              /* 2*a(i-6) */
            neg += cache[i - 10];             /* a(i-9) */

            cache[i - 1] = pos - neg;         /* pos >= neg */
        }
    }

    cache_size = n;
    return cache[n - 1];
}
