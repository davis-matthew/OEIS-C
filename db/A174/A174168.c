/*
 * A174168  A (1,3) Somos-4 sequence.
 * https://oeis.org/A174168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (a(n-1)*a(n-3) + 3*a(n-2)^2) / a(n-4)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174168 */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#endif

/* Initial values: a(0)=1 , a(1)=2 , a(2)=5 , a(3)=17 */
static const seq_t init_vals[4] = { (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)17 };

/* Dynamic cache for memoisation */
static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* allocated size of cache */
static size_t last_computed = (size_t)-1; /* largest n with cached value */

/* Ensure cache covers indices 0 .. n. */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_len >= needed)
        return;
    size_t new_len = cache_len ? cache_len : 8;
    while (new_len < needed)
        new_len <<= 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache_len = new_len;
}

/* Return a(n) for any n ≥ 0; uses memoisation. */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n <= 3)
        return init_vals[(size_t)n];

    /* Initialise cache on first call. */
    if (!cache) {
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (size_t i = 0; i < 4; ++i)
            cache[i] = init_vals[i];
        last_computed = 3;
    } else if (last_computed > (size_t)n) {
        /* Request smaller index – truncate the used range. */
        last_computed = (size_t)n;
    }

    /* Make sure there is room for the requested index. */
    ensure_cache_size(n);

    /* Compute any missing terms up to n. */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t value;

#if defined(__SIZEOF_INT128__)
#ifdef __cplusplus
        if constexpr (std::is_same<seq_t, uint64_t>::value) {
            u128 t1 = (u128)a1 * (u128)a3;
            u128 t2 = (u128)3 * (u128)a2 * (u128)a2;
            u128 num = t1 + t2;
            value = (seq_t)(num / (u128)a4);
        } else {
            value = (a1 * a3 + (seq_t)3 * a2 * a2) / a4;
        }
#else
        /* C: use 128‑bit intermediates. */
        u128 t1 = (u128)a1 * (u128)a3;
        u128 t2 = (u128)3 * (u128)a2 * (u128)a2;
        u128 num = t1 + t2;
        value = (seq_t)(num / (u128)a4);
#endif
#else
        /* No 128‑bit support – rely on seq_t operations (works for big‑int). */
        value = (a1 * a3 + (seq_t)3 * a2 * a2) / a4;
#endif

        cache[i] = value;
    }
    last_computed = (size_t)n;
    return cache[(size_t)n];
}
