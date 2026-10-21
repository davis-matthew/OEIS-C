/*
 * A096080  a(n) = floor((a(n-2)*a(n-1))/(a(n-1)+a(n-2))) + a(n-1) + a(n-2),
 *          a(0) = 0, a(1) = 1, a(2) = 1, ...
 * https://oeis.org/A096080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((a(n-2)*a(n-1))/(a(n-1)+a(n-2))) + a(n-1) + a(n-2),
 *            a(0)=0, a(1)=1, a(2)=1 The function memoises computed values and
 *            uses a 128‑bit intermediate when compiling for native integer
 *            types. For arbitrary‑precision builds the multiplication and
 *            division use the big‑int operators
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper to compute floor((x*y)/(x+y)) safely */
#ifdef __cplusplus
#ifdef __SIZEOF_INT128__
inline uint64_t compute_floor_prod_div(uint64_t x, uint64_t y) {
    unsigned __int128 prod = (unsigned __int128)x * y;
    unsigned __int128 sum = (unsigned __int128)x + y;
    return (uint64_t)(prod / sum);
}
#endif
/* Generic fallback for arbitrary‑precision or types without 128‑bit support */
template <typename T>
inline T compute_floor_prod_div(T x, T y) {
    return (x * y) / (x + y);
}
#else /* C mode */
#if defined(__SIZEOF_INT128__)
static inline seq_t compute_floor_prod_div(seq_t x, seq_t y) {
    unsigned __int128 prod = (unsigned __int128)x * y;
    unsigned __int128 sum = (unsigned __int128)x + y;
    return (seq_t)(prod / sum);
}
#else
/* If 128‑bit not available, fall back to simple integer arithmetic
   (may overflow for large values but is required for compilers lacking
   128‑bit support). */
static inline seq_t compute_floor_prod_div(seq_t x, seq_t y) {
    return (x * y) / (x + y);
}
#endif
#endif

static seq_t *cache = NULL;
static idx_t cache_len = 0;  /* number of values currently stored */
static size_t cache_cap = 0; /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* Allocate or extend cache to accommodate n */
    if ((size_t)n >= cache_cap) {
        if (cache_cap == 0) cache_cap = 16;
        while (cache_cap <= (size_t)n) cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Initialise cache with first three terms if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_len = 3;
    }

    /* Return already computed value */
    if (n < cache_len) return cache[(size_t)n];

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t x = cache[idx - 2];
        seq_t y = cache[idx - 1];
        seq_t div = compute_floor_prod_div(x, y);
        cache[idx] = div + y + x;
    }
    cache_len = n + 1;
    return cache[(size_t)n];
}
