/*
 * A137333  Spiral tiling numbers: union of {3*4^k, k>=0}, {4*9^k, k>=0}, and
 *          {6*7^k, k>=0}.
 * https://oeis.org/A137333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137333: Sorted merge of the three geometric sequences
   3*4^k, 4*9^k, and 6*7^k for k ≥ 0.  The function returns a(n)
   (a(1) = 3).  Duplicate values never arise, so a simple merge
   algorithm suffices. */
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values   */
static size_t cache_len = 0;     /* number of terms cached   */
static size_t cache_cap = 0;     /* current allocation size  */

static seq_t next3 = 3;          /* current 3*4^k             */
static seq_t next4 = 4;          /* current 4*9^k             */
static seq_t next6 = 6;          /* current 6*7^k             */

/* Ensure that cache holds at least n terms (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    /* Grow cache storage */
    if (cache_len >= (size_t)n) return;
    size_t need = (size_t)n;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate remaining terms */
    while (cache_len < need) {
        /* Find the smallest candidate */
        seq_t min_val = next3;
        if (next4 < min_val) min_val = next4;
        if (next6 < min_val) min_val = next6;

        cache[cache_len++] = min_val;

        /* Advance the sequence(s) that produced the min value */
        if (min_val == next3) next3 = next3 * (seq_t)4;
        if (min_val == next4) next4 = next4 * (seq_t)9;
        if (min_val == next6) next6 = next6 * (seq_t)7;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for offset handling */
    ensure_cache(n);
    return cache[n - 1];
}
