/*
 * A027013  a(n) = T(2*n, n+2), T given by A027011.
 * https://oeis.org/A027013
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A027013: a(n) = T(2*n, n+2), where T is A027011.
   Computation via linear recurrence derived from the generating function:
   (1 - 8x + 26x² - 45x³ + 45x⁴ - 26x⁵ + 8x⁶ - x⁷)·A(x) = x²
      *(1 + 69x² - 39x³ + 36x⁴ - 26x⁵ + 8x⁶ - x⁷)
   The recurrence is
        a(n) = 8 a(n‑1) – 26 a(n‑2) + 45 a(n‑3) – 45 a(n‑4)
             + 26 a(n‑5) – 8 a(n‑6) +   a(n‑7)   + Nₙ
   where Nₙ are the coefficients of the numerator
   (N₂ = 1, N₄ = 69, N₅ = –39, N₆ = 36, N₇ = –26, N₈ = 8, N₉ = –1, all other Nₙ = 0).  */

#include <stddef.h>
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

static seq_t *cache = NULL;          /* array storing computed a(n) values */
static size_t cache_cap = 0;         /* current allocated capacity */
static idx_t  cache_fill = 0;        /* index of first uncomputed entry (>= cache_fill-1 will be set) */

/* Coefficients of the numerator N(x) (indices 0..9) */
static const int N_coefs[10] = {0, 0, 1, 0, 69, -39, 36, -26, 8, -1};

/* Ensure cache has space for indices up to n inclusive. */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap == 0 ? 64 : cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;

    /* Initialize all new entries to 0 */
    for (size_t i = cache_fill; i < cache_cap; ++i) cache[i] = 0;

    /* Base values: a(0) = a(1) = 0 */
    if (cache_fill < 2) {
        cache[0] = 0;
        cache[1] = 0;
        cache_fill = 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* values below the offset are undefined – return 0 */

    ensure_cache(n);

    /* Compute any missing values up to n */
    for (idx_t i = cache_fill; i <= n; ++i) {
        seq_t pos = 0;           /* sum of positive contributions */
        seq_t neg = 0;           /* sum of negative contributions */

        if (i >= 1) pos += cache[i - 1] * (seq_t)8;
        if (i >= 3) pos += cache[i - 3] * (seq_t)45;
        if (i >= 5) pos += cache[i - 5] * (seq_t)26;
        if (i >= 7) pos += cache[i - 7];

        if (i >= 2) neg += cache[i - 2] * (seq_t)26;
        if (i >= 4) neg += cache[i - 4] * (seq_t)45;
        if (i >= 6) neg += cache[i - 6] * (seq_t)8;

        if (i <= 9) {
            int cn = N_coefs[i];
            if (cn > 0)
                pos += (seq_t)cn;
            else if (cn < 0)
                neg += (seq_t)(-cn);
        }

        cache[i] = pos - neg;    /* pos >= neg is guaranteed by the recurrence */
    }

    cache_fill = n + 1;
    return cache[n];
}
