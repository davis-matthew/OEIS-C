/*
 * A221021  Equals one maps: number of nX3 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal, vertical and antidiagonal neighbors in a random 0..3
 *          nX3 array.
 * https://oeis.org/A221021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) -30*a(n-2) -24*a(n-3) +63*a(n-4) +12*a(n-5)
 *            -32*a(n-6) for n>11 Initial values (a(1) .. a(12)) are
 *            hard‑coded below
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 84 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}
#endif

static seq_t *cache = NULL;          /* cache[0] == a(1) */
static idx_t cached = 0;             /* number of values currently cached */

static void ensure_cache(idx_t n)
{
    if (n <= cached) return;

    /* allocate or grow the cache */
    if (!cache) {
        cache = mem_new(seq_t, n);
        if (!cache) return;          /* allocation failure – silently ignore */
    } else if (n > cached) {
        seq_t *tmp = mem_resize(seq_t, cache, n);
        if (!tmp) return;
        cache = tmp;
    }

    /* initial known terms (a(1) .. a(12)) */
    static const seq_t init[12] = {
        (seq_t)4ULL,
        (seq_t)38ULL,
        (seq_t)371ULL,
        (seq_t)3474ULL,
        (seq_t)30167ULL,
        (seq_t)251674ULL,
        (seq_t)2055232ULL,
        (seq_t)16609480ULL,
        (seq_t)133546783ULL,
        (seq_t)1071058030ULL,
        (seq_t)8579199454ULL,
        (seq_t)68676536212ULL
    };

    idx_t i;
    for (i = cached + 1; i <= n; ++i) {
        if (i <= 12) {
            cache[i-1] = init[i-1];
        } else {
            /* recursion: a(n) = 12*a(n-1) -30*a(n-2) -24*a(n-3)
                            +63*a(n-4) +12*a(n-5) -32*a(n-6) */
            seq_t a1 = cache[i-2]; /* a(n-1) */
            seq_t a2 = cache[i-3]; /* a(n-2) */
            seq_t a3 = cache[i-4]; /* a(n-3) */
            seq_t a4 = cache[i-5]; /* a(n-4) */
            seq_t a5 = cache[i-6]; /* a(n-5) */
            seq_t a6 = cache[i-7]; /* a(n-6) */

#ifdef __cplusplus
            /* using big‑integer teacher utilities (works for arbitrary‑precision) */
            seq_t pos = mul_small(a1, 12) + mul_small(a4, 63) + mul_small(a5, 12);
            seq_t neg = mul_small(a2, 30) + mul_small(a3, 24) + mul_small(a6, 32);
            cache[i-1] = pos - neg;
#else
            /* for built‑in integer types – use signed __int128 to avoid overflow */
            unsigned __int128 pos = (__int128)a1 * 12 + (__int128)a4 * 63 + (__int128)a5 * 12;
            unsigned __int128 neg = (__int128)a2 * 30 + (__int128)a3 * 24 + (__int128)a6 * 32;
            unsigned __int128 val = pos - neg;
            cache[i-1] = (seq_t)val;
#endif
        }
    }
    cached = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n-1];
}
