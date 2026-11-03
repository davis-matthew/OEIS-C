/*
 * A229133  Numbers k such that the distance between the k-th triangular
 *          number and the nearest square is a square.
 * https://oeis.org/A229133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that the distance between the
 *            k-th triangular number T(k)=k*(k+1)/2 and the nearest square is
 *            itself a square. Since there is no simple closed form, the
 *            function generates terms by scanning k in increasing order,
 *            caching results so that subsequent calls are fast. T...
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Utility: floor square root for seq_t, provided by the framework */
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_len = 0;         /* number of stored terms */
static size_t cache_cap = 0;         /* current allocation size */
static idx_t last_tested = 0;        /* largest k already examined */

/* Append a value to the dynamic cache, growing as necessary. */
static void append_value(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();        /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Check whether an arbitrary-precision diff is a perfect square. */
static bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe to return 0 */

    /* Return precomputed value if it exists */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate terms until we have at least n of them */
    for (idx_t k = last_tested + 1;; ++k) {
        /* Triangular number T(k) = k*(k+1)/2, calculated in seq_t */
        seq_t Tk = ((seq_t)k * ((seq_t)k + 1)) / 2;

        /* Find the nearest square to Tk */
        seq_t s = floor_sqrt(Tk);                /* floor(sqrt(Tk)) */
        seq_t lower = s * s;                     /* s² <= Tk */
        seq_t diff_low = Tk - lower;             /* Tk - s² */
        seq_t diff_high = (s << 1) + 1 - diff_low; /* (s+1)² - Tk = 2*s+1 - diff_low */

        seq_t diff = (diff_low <= diff_high) ? diff_low : diff_high;

        /* If the distance is a square, store k */
        if (is_square(diff))
            append_value((seq_t)k);

        /* Stop once we have enough cached terms */
        if (cache_len >= n) {
            last_tested = k; /* k is the last examined value */
            break;
        }
    }

    return cache[n - 1];
}
