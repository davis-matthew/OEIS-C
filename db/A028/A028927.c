/*
 * A028927  Numbers represented by quadratic form with Gram matrix [ 3, 1; 1,
 *          5 ].
 * https://oeis.org/A028927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A028927
   A(n) = smallest non‑negative integer representable by
   3*x^2 + 2*x*y + 5*y^2 with x >= 0 and y any integer.
   The implementation searches successive integers and caches the
   values so that calls in arbitrary order remain fast. */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */

/* Integer square root for idx_t (uint64_t). */
static idx_t isqrt_u64(idx_t n) {
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* highest power of four <= 2^64-1 */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Cache storage.  All function mutating the cache are static helpers. */
static seq_t *cache = NULL;  /* array of seq_t */
static idx_t cache_len = 0;  /* number of elements in cache */

/* Append a new value to the cache, reallocating if needed. */
static void append_cache(seq_t value) {
    static size_t capacity = 0;
    if (cache_len >= capacity) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_mem) { /* allocation failed – abort silently */
            free(cache);
            cache = NULL;
            capacity = 0;
            cache_len = 0;
            return;
        }
        cache = new_mem;
        capacity = new_cap;
    }
    cache[cache_len++] = value;
}

/* Check whether v can be represented by 3*x^2 + 2*x*y + 5*y^2. */
static bool is_representable(idx_t v) {
    /* max_x from 5*v/14 >= x^2 -> x <= sqrt(5*v/14) */
    idx_t max_x = isqrt_u64((5ULL * v) / 14ULL);
    for (idx_t x = 0; x <= max_x; ++x) {
        /* Compute D = 20*v - 56*x^2. Use 128-bit to avoid overflow. */
        unsigned __int128 left  = (unsigned __int128)20 * v;
        unsigned __int128 right = (unsigned __int128)56 * x * x;
        if (right > left) continue;            /* D would be negative */
        unsigned __int128 d128 = left - right;
        if (d128 > (unsigned __int128)UINT64_MAX) continue;
        idx_t D = (idx_t)d128;
        idx_t t = isqrt_u64(D);
        if (t * t != D) continue;              /* D not a perfect square */

        /* Check y = (-2*x ± t) / 10 is integer. */
        for (int sgn = -1; sgn <= 1; sgn += 2) {
            int64_t num = -(int64_t)2 * (int64_t)x + sgn * (int64_t)t;
            if (num % 10 == 0) return true;
        }
    }
    return false;
}

/* Ensure cache contains at least n elements. */
static void ensure_cache(idx_t n) {
    if (n <= cache_len) return;   /* already enough */
    idx_t v = (cache_len == 0) ? 0 : (idx_t)cache[cache_len - 1] + 1;
    while (cache_len < n) {
        while (true) {
            if (is_representable(v)) {
                append_cache((seq_t)v);
                break;
            }
            ++v;
        }
    }
}

/* Main entry point – public function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined, guard against underflow */
    ensure_cache(n);
    return cache[n - 1];
}
