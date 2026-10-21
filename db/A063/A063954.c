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
 * Helpers:   is_sum_of_three_squares, ok_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_sum_of_three_squares.h"
#include "../utils/ok_value.h"

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
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
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
