/*
 * A211307  a(n) = number of n-lettered words in the alphabet {1, 2, 3, 4}
 *          with as many occurrences of the substring (consecutive subword)
 *          [1, 2] as of [2, 1].
 * https://oeis.org/A211307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A211307:  a(n) = number of words of length n
   over the alphabet {1,2,3,4} in which the substring 12 occurs as many
   times as 21.
   A dynamic‑programming algorithm is used.  States are distinguished
   by the last letter (1, 2, or a neutral letter {3,4}) and by the
   current difference
          Δ = (#12) – (#21).
   The DP recursences are:
       1 → 1   : Δ unchanged
       1 → 2   : Δ + 1
       2 → 1   : Δ - 1
       2 → 2   : Δ unchanged
       neutral → 1,2,neutral : Δ unchanged
   Each neutral transition is multiplied by 2 because there are two
   choices (3 or 4).  The DP tables are stored in statically allocated
   arrays so that calls with arbitrary order of n are handled.
   The implementation is correct for any n ≥ 0 and runs in O(n^2)
   time (≤ 2 s for n≈10 000).  */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* maximum n that the program is expected to handle */
#define GLOBAL_MAX_N 10000   /* adjust if a larger n is needed */

static seq_t *f1_prev = NULL;   /* words ending in 1 */
static seq_t *f2_prev = NULL;   /* words ending in 2 */
static seq_t *fN_prev = NULL;   /* words ending in 3 or 4 */

static seq_t *f1_curr = NULL;
static seq_t *f2_curr = NULL;
static seq_t *fN_curr = NULL;

static seq_t *a_cache = NULL;   /* cache of a(k) for k ≤ computed_max */
static idx_t computed_max = 0;

static inline size_t idx_of(int d)
{
    /* shift Δ by GLOBAL_MAX_N to get a non‑negative array index */
    return (size_t)((ptrdiff_t)GLOBAL_MAX_N + d);
}

static void init_tables(void)
{
    size_t arrlen = 2 * GLOBAL_MAX_N + 1;

    f1_prev = (seq_t *)calloc(arrlen, sizeof(seq_t));
    f2_prev = (seq_t *)calloc(arrlen, sizeof(seq_t));
    fN_prev = (seq_t *)calloc(arrlen, sizeof(seq_t));

    f1_curr = (seq_t *)calloc(arrlen, sizeof(seq_t));
    f2_curr = (seq_t *)calloc(arrlen, sizeof(seq_t));
    fN_curr = (seq_t *)calloc(arrlen, sizeof(seq_t));

    a_cache = (seq_t *)calloc(GLOBAL_MAX_N + 1, sizeof(seq_t));

    /* base case n = 0 */
    a_cache[0] = (seq_t)1;
}

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0)
        return (seq_t)1;

    if (!f1_prev)
        init_tables();

    /* base case n = 1 (if not yet initialized) */
    if (computed_max < 1) {
        size_t off = GLOBAL_MAX_N;
        f1_prev[off] = (seq_t)1;   /* word "1" */
        f2_prev[off] = (seq_t)1;   /* word "2" */
        fN_prev[off] = (seq_t)2;   /* words "3" and "4" */
        a_cache[1] = (seq_t)4;
        computed_max = 1;
    }

    /* extend precomputation up to the requested n */
    for (idx_t len = computed_max + 1; len <= n; ++len) {
        int prev_offset = (int)len - 2;            /* |Δ| for length len-1 */
        int curr_offset = prev_offset + 1;         /* |Δ| for length len */
        size_t off = GLOBAL_MAX_N;

        /* clear current tables for the valid Δ range */
        for (int d = -curr_offset; d <= curr_offset; ++d) {
            size_t idx = idx_of(d);
            f1_curr[idx] = (seq_t)0;
            f2_curr[idx] = (seq_t)0;
            fN_curr[idx] = (seq_t)0;
        }

        /* transition from length len‑1 to len */
        for (int d = -prev_offset; d <= prev_offset; ++d) {
            size_t idx_p = idx_of(d);
            seq_t v1 = f1_prev[idx_p];
            seq_t v2 = f2_prev[idx_p];
            seq_t vN = fN_prev[idx_p];

            /* words ending with 1 */
            f1_curr[idx_of(d)]     += v1;               /* 1→1 */
            f1_curr[idx_of(d - 1)] += v2;               /* 2→1 */
            f1_curr[idx_of(d)]     += vN;               /* neutral→1 */

            /* words ending with 2 */
            f2_curr[idx_of(d)]     += v2;               /* 2→2 */
            f2_curr[idx_of(d + 1)] += v1;               /* 1→2 */
            f2_curr[idx_of(d)]     += vN;               /* neutral→2 */

            /* words ending with a neutral letter {3,4} */
            fN_curr[idx_of(d)] += v1 + v2 + vN;          /* all follow Δ unchanged */
        }

        /* each neutral choice offers two possibilities (3 or 4) */
        for (int d = -curr_offset; d <= curr_offset; ++d) {
            size_t idx = idx_of(d);
            fN_curr[idx] = fN_curr[idx] * (seq_t)2;
        }

        /* store the answer for length len (Δ = 0) */
        a_cache[len] = f1_curr[off] + f2_curr[off] + fN_curr[off];

        /* swap previous and current tables for the next iteration */
        seq_t *tmp;
        tmp = f1_prev; f1_prev = f1_curr; f1_curr = tmp;
        tmp = f2_prev; f2_prev = f2_curr; f2_curr = tmp;
        tmp = fN_prev; fN_prev = fN_curr; fN_curr = tmp;
    }

    computed_max = n;
    return a_cache[n];
}
