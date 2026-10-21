/*
 * A090467  Numbers which are not regular figurative or polygonal numbers of
 *          order greater than 2. That is, numbers not of the form 1 +
 *          k*m*(m-1)/2 - (m-1)^2 where k > 2 and m > 2.
 * https://oeis.org/A090467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 9452, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_polygonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A090467: Numbers that are not k‑gonal (k>2) with m>2.
   For an integer n ≥ 1, n is in the sequence iff there are no integers
   m ≥ 3 and k ≥ 3 such that
          n = 1 + k*m*(m-1)/2 - (m-1)^2 .
   We generate the irregular numbers by brute force, testing each
   candidate for polygonality.  The test runs in O(√n) time per number.   */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_polygonal.h"

/* ---------------- Cache for irregular numbers -------------------------------- */
static seq_t *irreg_cache = NULL;   /* array of computed terms   */
static size_t irreg_cap   = 0;      /* allocated capacity         */
static size_t irreg_len   = 0;      /* number of terms stored     */
static seq_t  next_candidate = 1;   /* next integer to test       */

/* Grow internal cache to at least 'needed' elements. */
static void ensure_irreg_capacity(size_t needed)
{
    if (irreg_cap < needed) {
        size_t new_cap = irreg_cap ? irreg_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(irreg_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);   /* out of memory */
        irreg_cache = tmp;
        irreg_cap = new_cap;
    }
}

/* ---------------- Public function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset handling            */
    if ((size_t)n <= irreg_len)                /* cache hit                  */
        return irreg_cache[n - 1];

    /* Generate further irregular numbers until we reach the desired index. */
    while (irreg_len < (size_t)n) {
        seq_t val = next_candidate++;
        if (!is_polygonal(val)) {
            ensure_irreg_capacity(irreg_len + 1);
            irreg_cache[irreg_len++] = val;
        }
    }
    return irreg_cache[n - 1];
}
