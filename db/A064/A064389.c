/*
 * A064389  Variation (4) on Recamán's sequence (A005132): to get a(n), we
 *          first try to subtract n from a(n-1): a(n) = a(n-1) - n if positive
 *          and not already in the sequence; if not then we try to add n: a(n)
 *          = a(n-1) + n if not already in the sequence; if this fails we try
 *          to subtract n+1 from a(n-1), or to...
 * https://oeis.org/A064389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, nice
 * Formula:   OEIS A064389 – Variation (4) on Recamán's sequence. Recurrence:
 *            a(1) = 1. For n ≥ 2, let m start at n and increase by 1 until
 *            either a(n‑1) – m or a(n‑1) + m (or the next m) yields a
 *            positive number not yet in the sequence
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: The code works for both C11 and C++17.  In C++ the result of
   malloc / calloc / realloc is cast to the appropriate pointer
   type so that default construction of seq_t objects (if any) is
   left untouched – the build assumes seq_t is trivially default
   constructible. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* static state: memoised sequence values and presence bitmap */
static seq_t *seq_vals = NULL; /* 1‑based array a(n) */
static idx_t  seq_cap = 0;     /* current capacity of seq_vals (>= index+1) */

static bool *used = NULL;      /* presence flag for values seen */
static idx_t  used_cap = 0;    /* current capacity of used array */

/* Ensure the presence array can hold index `idx`. */
static void extend_used(idx_t idx)
{
    if (idx < used_cap) return;
    idx_t new_cap = idx + 1;
    void *p = realloc(used, new_cap * sizeof(bool));
    if (!p) abort();
    used = (bool *)p;
    memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used_cap = new_cap;
}

/* Ensure the sequence array can hold element at position `idx`. */
static void extend_seq(idx_t idx)
{
    if (idx < seq_cap) return;
    idx_t new_cap = idx;
    void *p = realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!p) abort();
    seq_vals = (seq_t *)p;
    seq_cap = new_cap;
}

/* The required sequence generator.  It may be called in any order
   of indices; missing terms are computed on demand and cached. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but avoid UB */

    /* Initialisation on first call */
    if (seq_vals == NULL) {
        seq_vals = (seq_t *)calloc(2, sizeof(seq_t)); /* indices 0 and 1 */
        seq_cap  = 2;
        seq_vals[1] = (seq_t)1;                       /* a(1) = 1 */

        used      = (bool *)calloc(2, sizeof(bool));
        used_cap  = 2;
        used[1]   = true; /* mark 1 as used */
    }

    /* Compute terms up to requested index n */
    for (idx_t i = seq_cap; i <= n; ++i) {
        seq_t prev = seq_vals[i - 1];
        seq_t m    = (seq_t)i;          /* start candidate shift */
        seq_t cand = 0;
        bool found = false;

        while (!found) {
            /* try a(n-1) - m if positive and unused */
            if (prev > m) {
                seq_t minus = prev - m;
                if (minus > 0) {
                    idx_t idx_minus = (idx_t)minus;
                    if (!(idx_minus < used_cap && used[idx_minus])) {
                        cand   = minus;
                        found = true;
                        break;
                    }
                }
            }

            /* try a(n-1) + m */
            seq_t plus  = prev + m;
            idx_t idx_plus = (idx_t)plus;
            if (idx_plus >= used_cap) extend_used(idx_plus + 1);
            if (!used[idx_plus]) {
                cand   = plus;
                found = true;
                break;
            }

            /* increase m and try again */
            ++m;
        }

        /* Store new term */
        extend_seq(i + 1);              /* ensure capacity for a(i) */
        seq_vals[i] = cand;

        idx_t idx_cand = (idx_t)cand;
        if (idx_cand >= used_cap) extend_used(idx_cand + 1);
        used[idx_cand] = true;
    }

    return seq_vals[n];
}
