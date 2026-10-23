/*
 * A174650  Expansion of 1 / (1 - x - x^2 + x^3 - x^5 + x^15 - x^17 - x^18 +
 *          x^19 + x^20).
 * https://oeis.org/A174650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 61, u64 n <= 122,
 *            u128 n <= 243, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from the rational generating function of A174650:
   a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=3, a(5)=4, a(6)=6, a(7)=9,
   a(8)=13, a(9)=19, a(10)=27, a(11)=39, a(12)=56, a(13)=81, a(14)=117,
   a(15)=168, a(16)=242, a(17)=348, a(18)=503, a(19)=725, a(20)=1046.
   For n≥21:
   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-5)-a(n-15)+a(n-17)+a(n-18)-a(n-19)-a(n-20). */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system. */

static seq_t *cache = NULL;   /* cached sequence values */
static size_t cached = 0;     /* number of values stored in cache */
static size_t capacity = 0;   /* allocated capacity for cache */

/* first 21 terms of the sequence */
static const seq_t init_vals[21] = {
    1ULL, 1ULL, 2ULL, 2ULL, 3ULL, 4ULL, 6ULL, 9ULL,
    13ULL, 19ULL, 27ULL, 39ULL, 56ULL, 81ULL, 117ULL,
    168ULL, 242ULL, 348ULL, 503ULL, 725ULL, 1046ULL
};

/* enlarge the cache to hold at least 'need+1' elements (0‑based indices) */
static void ensure_capacity(size_t need)
{
    if (capacity > need) return;          /* already enough space */

    size_t new_cap = capacity ? capacity << 1 : 32;
    if (new_cap <= need) new_cap = need + 1;

    seq_t *tmp;
#ifdef __cplusplus
    tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = tmp;
    capacity = new_cap;

    /* initialise the first 21 values on first use */
    if (cached == 0 && capacity > 0) {
        for (size_t i = 0; i < 21 && i < capacity; ++i)
            cache[i] = init_vals[i];
        cached = 21;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_capacity(idx);

    /* compute any missing terms up to idx */
    while (cached <= idx) {
        /* recurrence needs indices up to n-20; cached >= 21 guarantees all are available */
        seq_t val = cache[cached - 1] + cache[cached - 2]
                    - cache[cached - 3] + cache[cached - 5]
                    - cache[cached - 15] + cache[cached - 17]
                    + cache[cached - 18] - cache[cached - 19]
                    - cache[cached - 20];
        cache[cached++] = val;
    }

    return cache[idx];
}
