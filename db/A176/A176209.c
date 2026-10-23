/*
 * A176209  Numbers that can be greedily decomposed as a sum of two or more
 *          squares >= 4.
 * https://oeis.org/A176209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that can be greedily decomposed as a sum of two or more
 * squares ≥ 4.
 *
 * For any integer x > 0, let greedy_can(x) test whether the greedy
 * decomposition algorithm (repeatedly subtract the largest square ≤ remainder)
 * uses at least two summands.  Then a(1), a(2), … are the successive
 * integers with greedy_can(x) == true.  This code generates the terms on
 * demand and caches them for repeated queries.
 *
 * Formula: a(n) is the n‑th integer that can be greedily expressed as a sum of
 * at least two squares each ≥ 4. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- Integer square‑root (floor) for idx_t ---------------------------------- */
static idx_t floor_sqrt_idx(idx_t n)
{
    if (n < 2)
        return n;
    idx_t low = 0, high = (n >> 1) + 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        /* use 128‑bit to avoid overflow of mid*mid */
        __uint128_t prod = (__uint128_t)mid * (__uint128_t)mid;
        if (prod <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* ---- Test whether x can be greedily decomposed into ≥2 squares ≥4 --------- */
static bool greedy_can(seq_t x)
{
    seq_t rem = x;
    seq_t count = 0;
    while (rem > 0) {
        /* safe cast: rem ≤ maximum value that fits in seq_t,
         * which is ≤ 2^64-1, hence fits in idx_t. */
        idx_t r = (idx_t)rem;
        idx_t s = floor_sqrt_idx(r);
        if (s < 2)            /* square < 4 */
            return false;
        seq_t sq = (seq_t)s * (seq_t)s;
        rem -= sq;
        ++count;
    }
    return count >= 2;
}

/* ---- Cache to store discovered terms ------------------------------------- */
static seq_t *cache = NULL;      /* array of found terms     */
static idx_t cache_len = 0;      /* number of terms stored   */
static idx_t cache_cap = 0;      /* allocated capacity       */

/* ---- Ensure cache contains at least n terms ------------------------------ */
static void ensure_terms(idx_t n)
{
    if (cache_len >= n)
        return;

    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    seq_t cand = (cache_len == 0) ? (seq_t)8 : cache[cache_len - 1] + 1;

    while (cache_len < n) {
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        if (greedy_can(cand))
            cache[cache_len++] = cand;
        ++cand;
    }
}

/* ---- Public entry point --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 1, safe fallback */
    ensure_terms(n);
    return cache[n - 1];
}
