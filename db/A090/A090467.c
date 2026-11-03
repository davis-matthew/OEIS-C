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
 * Validated: standard, pass, 81 terms through n = 9453 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* ---------------- Helper: test whether a value is k‑gonal with k>2, m>2 --------- */
static bool is_polygonal(seq_t n)
{
    /* We only need to consider m up to roughly 4·sqrt(n); this guarantees
       that left/ t can never reach 3 again, hence no larger m can produce
       k ≥ 3.  The loop terminates when (m−1)² > 4·n. */
    for (idx_t m = 3; ; ++m) {
        seq_t m1 = (seq_t)m - 1;
        /* Break early when (m−1)² > 4·n. */
        if (m1 * m1 > (n << 2)) break;

        seq_t t    = (seq_t)m * m1 / 2;          /* m·(m−1)/2   */
        seq_t left = n + m1 * m1 - 1;            /* n + (m−1)² −1 */

        if (left % t == 0) {
            seq_t k = left / t;
            if (k > 2) return true;             /* k ≥ 3 found */
        }
    }
    return false;                                 /* no suitable (k,m) */
}

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
        seq_t *tmp = mem_resize(seq_t, irreg_cache, new_cap);
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
