/*
 * A162909  Numerators of Bird tree fractions.
 * https://oeis.org/A162909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(2^m+k) = a(2^m-k-1), a(2^m+2^(m-1)+k) = a(2^m+k) +
 *            a(2^(m-1)+k), a(1) = 1
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* memoisation array */
static idx_t cache_size = 0;  /* current capacity of cache */

/* floor(log2(x)) for x > 0 */
static inline idx_t floor_log2(idx_t x) { return 63 - __builtin_clzll(x); }

/* recursive computation with memoisation */
static seq_t calc(idx_t n)
{
    if (n < cache_size && cache[n] != 0)
        return cache[n];

    seq_t val;
    if (n == 1) {
        val = 1;
    } else {
        idx_t p   = 1ULL << floor_log2(n);   /* largest power of two ≤ n */
        idx_t t   = n - p;                   /* offset within the row */
        idx_t half = p >> 1;                 /* 2^(m-1) */

        if (t < half) {
            val = calc(p - t - 1);           /* symmetry */
        } else {
            idx_t left  = p + t - half;      /* a(2^m+k) */
            idx_t right = (p >> 1) + (t - half); /* a(2^(m-1)+k) */
            val = calc(left) + calc(right);  /* recurrence */
        }
    }

    /* grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = (cache_size == 0) ? 1 : cache_size;
        while (new_size <= n) new_size <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;                                 /* initial value */
        cache = tmp;
        cache_size = new_size;
    }

    cache[n] = val;
    return val;
}

/* external function required by the judge */
seq_t a(idx_t n)
{
    return calc(n);
}
