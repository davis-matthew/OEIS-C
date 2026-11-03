/*
 * A063954  Every number is the sum of 4 squares; these are the odd numbers
 *          such that the first square can be taken to be any square < n.
 * https://oeis.org/A063954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, fini, full
 * Formula:   a(n) = the nth odd integer such that for each perfect square k²
 *            < a(n), the number a(n) − k² can be expressed as a sum of three
 *            squares (including zeros). Equivalently, for every k with 0 ≤ k²
 *            < a(n), a(n) – k² is NOT of the form 4^α(8β + 7)
 * Range:     u8 n <= 29, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: full, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: true iff m can be expressed as a sum of three squares.  */
static int is_sum_of_three_squares(seq_t m)
{
    seq_t x = m;
    while (x % 4 == 0) x /= 4;
    return (x % 8 != 7);
}

/* Helper: true iff odd number v satisfies the defining property. */
static int ok_value(seq_t v)
{
    /* k = 0 ------------------------------------------------- */
    if (!is_sum_of_three_squares(v)) return 0;

    /* k >= 1 ------------------------------------------------ */
    for (seq_t k = 1; ; ++k) {
        /* Stop when k² would exceed v−1.  Avoid overflow by comparing
           (v−1)/k < k, which is equivalent to k² > v−1.            */
        if (k > 0 && (v - 1) / k < k) break;

        seq_t sq = k * k;
        seq_t r  = v - sq;
        if (!is_sum_of_three_squares(r)) return 0;
    }
    return 1;
}

/* Cache of already computed sequence terms. */
static seq_t  *cache = NULL;
static idx_t   size  = 0;  /* number of terms stored */
static idx_t   cap   = 0;  /* allocated capacity */

/* Ensure cache can hold at least 'need' elements. */
static void ensure_cap(idx_t need)
{
    if (need <= cap) return;
    idx_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    cache = mem_resize(seq_t, cache, new_cap);
    cap   = new_cap;
}

/* Generate terms until we have at least 'n' elements in the cache. */
static void fill_cache_until(idx_t n)
{
    if (size >= n) return;

    /* Start searching from the next odd candidate after the last stored. */
    seq_t cand = (size == 0) ? 1 : cache[size - 1] + 2;

    while (size < n) {
        if (ok_value(cand)) {
            ensure_cap(size + 1);
            cache[size++] = cand;
        }
        cand += 2;   /* next odd number */
    }
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n=0, but avoid crash */
    fill_cache_until(n);
    return cache[n - 1];
}
