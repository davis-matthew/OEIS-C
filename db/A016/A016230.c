/*
 * A016230  Expansion of 1/((1-x)(1-5x)(1-7x)).
 * https://oeis.org/A016230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=13, a(n)=12*a(n-1)-35*a(n-2)+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for the sequence values */
static seq_t *cache = NULL;
static size_t cached = 0;   /* number of terms already computed */
static size_t capacity = 0;

/* Ensure the cache can hold at least (n+1) entries */
static void _ensure_capacity(size_t n)
{
    if (capacity >= n + 1)
        return;
    size_t newcap = capacity ? capacity * 2 : 4;
    while (newcap < n + 1)
        newcap *= 2;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    capacity = newcap;
}

/* Return a(n) for the OEIS sequence A016230 */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Prime initialization of the cache */
    if (cache == NULL) {
        capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = 1;      /* a(0) */
        cache[1] = 13;     /* a(1) */
        cached = 2;
    }

    /* Compute terms up to idx if necessary */
    while (cached <= idx) {
        _ensure_capacity(cached);
        seq_t val = 12 * cache[cached - 1] - 35 * cache[cached - 2] + 1;
        cache[cached] = val;
        cached++;
    }

    return cache[idx];
}
