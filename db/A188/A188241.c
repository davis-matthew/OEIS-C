/*
 * A188241  Number of nondecreasing arrangements of 8 numbers in -(n+6)..(n+6)
 *          with sum zero and not more than two numbers equal.
 * https://oeis.org/A188241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 51, u64 n <= 175, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A188241
 *  a(1)…a(23) are the given initials.
 *  a(n) = 3*a(n-1) - 2*a(n-2) - 3*a(n-4) + 4*a(n-5)
 *         - 3*a(n-8) + 3*a(n-9) - a(n-11) - a(n-12)
 *         + 3*a(n-14) - 3*a(n-15) + 4*a(n-18) - 3*a(n-19)
 *         - 2*a(n-21) + 3*a(n-22) - a(n-23)      (n ≥ 24) */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* 1‑based cache, cache[0] unused */
static idx_t cache_size = 0;     /* number of stored entries (≥0) */

static const seq_t init_23[23] = {
    5020, 11376, 23522, 45225, 81981,
    141519, 234413, 374820, 581280, 877662,
    1294252, 1868927, 2648493, 3690201, 5063359,
    6851134, 9152564, 12084676, 15784822, 20413257,
    26155829, 33226923, 41872667
};

/* helper to obtain a value that may be in the old or the new cache */
static inline seq_t _get_val(idx_t idx, seq_t *new_cache)
{
    return (idx <= cache_size) ? cache[idx] : new_cache[idx];
}

/* positive / negative coefficient list */
typedef struct { idx_t offs; int coeff; } pair_t;
static const pair_t coeffs[] = {
    { 1,  3}, { 2, -2}, { 4, -3}, { 5,  4},
    { 8, -3}, { 9,  3}, {11, -1}, {12, -1},
    {14,  3}, {15, -3}, {18,  4}, {19, -3},
    {21, -2}, {22,  3}, {23, -1}
};
static const size_t coeffs_cnt = sizeof(coeffs) / sizeof(coeffs[0]);

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */
    if (n <= cache_size) return cache[n];

    /* expand cache to hold indices up to n */
    size_t new_size = (size_t)n + 1;    /* indices 0 … n */
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) return 0;           /* allocation failure */

    /* copy existing values */
    for (size_t i = 1; i <= cache_size; ++i)
        new_cache[i] = cache[i];

    /* compute missing terms */
    for (size_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 23) {
            val = init_23[i - 1];
        } else {
#if defined(__UINT128_MAX__) && !defined(__SIZEOF_LONG_LONG__)   /* 64 bit case */
            /* 128‑bit intermediate to avoid overflow */
            __int128_t sum = 0;
            for (size_t k = 0; k < coeffs_cnt; ++k) {
                seq_t tmp = _get_val((idx_t)i - coeffs[k].offs, new_cache);
                int coeff = coeffs[k].coeff;
                if (coeff > 0)
                    sum += (__int128_t)(uint64_t)tmp * coeff;
                else
                    sum -= (__int128_t)(uint64_t)tmp * (-coeff);
            }
            val = (seq_t)sum;
#else
            /* arbitrary‑precision big integer: use signed addition/subtraction */
            seq_t pos = 0, neg = 0;
            for (size_t k = 0; k < coeffs_cnt; ++k) {
                seq_t tmp = _get_val((idx_t)i - coeffs[k].offs, new_cache);
                int coeff = coeffs[k].coeff;
                seq_t prod = tmp * (seq_t)(coeff > 0 ? coeff : -coeff);
                if (coeff > 0)
                    pos += prod;
                else
                    neg += prod;
            }
            val = pos - neg;
#endif
        }
        new_cache[i] = val;
    }

    /* free old cache and replace */
    free(cache);
    cache = new_cache;
    cache_size = n;

    return cache[n];
}
