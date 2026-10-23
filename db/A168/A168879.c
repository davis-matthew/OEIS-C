/*
 * A168879  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 1
 *   a(n) = 10 * 9^(n-1)          for 1 ≤ n ≤ 20
 *   a(n) = 8 * (a(n-1)+…+a(n-20))
 *         - 36 * a(n-21) + 1     for n ≥ 21
 *
 * This implements the recurrence derived from the generating function.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static buffer holding the sequence up to the largest n already computed */
static seq_t *cache      = NULL; /* array of terms */
static size_t cache_len  = 0;    /* number of terms stored (cache_len == 0 means uninitialised) */
static size_t cache_cap  = 0;    /* current allocation size */
static seq_t  sum20      = 0;    /* sum of the last 20 terms needed for the recurrence */
                                /* (valid only after cache_len >= 21) */

static void _ensure_cache(idx_t n)
{
    /* if we already have the requested terms, nothing to do */
    if (cache_len > n)
        return;

    /* ------------------------------------------------------------------
     *  Generate the initial 21 terms (0 … 20).  These follow the
     *  simple pattern 1, 10, 90, 810, … = 10*9^(n-1).         ----------
     ------------------------------------------------------------------ */
    if (cache_len < 21) {
        /* allocate space for at least 21 terms */
        if (cache_cap < 21) {
            cache = (seq_t *)realloc(cache, 21 * sizeof(seq_t));
            cache_cap = 21;
        }

        cache[0] = (seq_t)1;
        seq_t pow9 = (seq_t)10;           /* 10*9^(1-1) for n = 1        */
        cache[1] = pow9;
        for (idx_t i = 2; i <= 20; ++i) {
            pow9 = pow9 * (seq_t)9;       /* 10*9^(i-1)                  */
            cache[i] = pow9;
        }

        /* store the number of computed terms so far */
        cache_len = 21;

        /* sum of the last 20 terms, i.e. a(1) … a(20) */
        sum20 = (seq_t)0;
        for (idx_t i = 1; i <= 20; ++i)
            sum20 += cache[i];
    }

    /* ------------------------------------------------------------------
     *  Extend the cache to at least n+1 terms using the recurrence.
     ------------------------------------------------------------------ */
    if (n + 1 > cache_cap) {
        /* enlarge the allocation */
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 8*sum20 - 36*a(i-21) + 1   (i ≥ 21) */
        seq_t term = (seq_t)8 * sum20 - (seq_t)36 * cache[i - 21] + (seq_t)1;
        cache[i] = term;

        /* update the sliding window sum for the next step */
        sum20 = sum20 + term - cache[i - 20];
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    _ensure_cache(n);
    return cache[n];
}
