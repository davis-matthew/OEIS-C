/*
 * A175409  Successive numbers of consecutive positive terms to add when
 *          rearranging the alternating harmonic series to sum to log[7/3].
 * https://oeis.org/A175409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A175409 by simulating the alternating
 * harmonic series 1‑1/2+1/3-1/4+… with the threshold s = log(7/3).
 * Each term a(n) is the number of consecutive positive odd‑term
 * fractions added before the partial sum exceeds s, after every
 * subtraction of consecutive negative even‑term fractions that
 * brings the sum below s.  The function memoizes results so it
 * works correctly for arbitrary call order. */

/* Note: seq_t and idx_t are typedefs supplied by the build system.
 * idx_t is an unsigned integer type (e.g. uint64_t). */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Target sum s = log(7/3) computed once. */
static const long double target = logl(7.0L / 3.0L);

/* Cached sequence values: a(1), a(2), … */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

/* Current state of the alternating series simulation. */
static long double running_sum = 0.0L;
static idx_t pos_idx = 1;   /* next positive term index (denominator = 2*pos_idx-1) */
static idx_t neg_idx = 1;   /* next negative term index (denominator = 2*neg_idx) */

/* Ensure the cache has at least 'needed' slots. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main entry point required by the OEIS framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* index 0 is out of domain */

    /* Generate terms up to n, memoizing as we go. */
    while (cache_size < (size_t)n)
    {
        /* ---------- Positive block ----------
         * Add odd‑term fractions until the partial sum exceeds the target. */
        size_t start_pos = pos_idx;
        while (running_sum <= target)
        {
            running_sum += 1.0L / (2.0L * (long double)pos_idx - 1.0L);
            ++pos_idx;
        }
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size++] = (seq_t)(pos_idx - start_pos);

        /* ---------- Negative block ----------
         * Subtract even‑term fractions until the partial sum drops below the target. */
        while (running_sum >= target)
        {
            running_sum -= 1.0L / (2.0L * (long double)neg_idx);
            ++neg_idx;
        }
    }

    return cache[n - 1];
}
