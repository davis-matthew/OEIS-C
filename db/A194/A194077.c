/*
 * A194077  Natural interspersion of A060432; a rectangular array, by
 *          antidiagonals.
 * https://oeis.org/A194077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (i,j)-entry of the infinite array defined by a
 *            natural interspersion of A060432. For a given n, its diagonal
 *            index t is the smallest integer with t(t+1)/2 ≥ n. The row and
 *            column are i = n – t(t-1)/2 j = t – i + 1 Row i starts at s₁ =
 *            1, s₂ = 2, sᵢ = sᵢ₋₁ + (i-1)² + 1 for i ≥ 3 The j‑...
 * Range:     u8 n <= 54, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Utility for triangular numbers ---------- */
static inline idx_t triangular(idx_t k)
{
    return k * (k + 1) / 2;
}

/* ---------- Cache for row starting values ---------- */
static idx_t start_last = 0;
static size_t start_cache_cap = 0;
static seq_t *start_cache = NULL;

static void ensure_start(idx_t need)
{
    if (need <= start_last)
        return;
    size_t old_cap = start_cache_cap;
    size_t new_cap = old_cap ? old_cap * 2 : 4;
    while (new_cap <= (size_t)need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(start_cache, new_cap * sizeof(seq_t));
    if (!tmp) /* out of memory: abort */
        abort();
    start_cache = tmp;
    start_cache_cap = new_cap;
    for (idx_t k = start_last + 1; k <= need; ++k) {
        if (k == 1)
            start_cache[1] = (seq_t)1;
        else if (k == 2)
            start_cache[2] = (seq_t)2;
        else {
            seq_t prev = start_cache[k - 1];
            seq_t add  = (seq_t)((k - 1) * (k - 1)) + (seq_t)1;
            start_cache[k] = prev + add;
        }
    }
    start_last = need;
}

/* ---------- Compute a(i,j) ---------- */
static inline seq_t row_value(idx_t i, idx_t j)
{
    ensure_start(i);
    seq_t v = start_cache[i];
    if (j <= 1)
        return v;

    /* Difference pattern */
    seq_t diff   = (i >= 2) ? (seq_t)i : (seq_t)2;   /* starting d */
    idx_t  rem   = (idx_t)diff;                       /* times left for this d */
    idx_t  steps = j - 1;                             /* number of diffs needed */

    while (steps > 0) {
        idx_t use = (steps < rem) ? steps : rem;
        v += diff * (seq_t)use;
        steps -= use;
        rem   -= use;
        if (rem == 0) {
            diff = diff + (seq_t)1;
            rem  = (idx_t)diff;
        }
    }
    return v;
}

/* ---------- Public function a(idx_t n) ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n=0 */

    /* Find diagonal t: minimal t with T(t) >= n */
    idx_t t = 1;
    while (triangular(t) < n)
        ++t;

    idx_t prev = triangular(t - 1);   /* T(t-1) */
    idx_t i    = n - prev;            /* row index (1‑based) */
    idx_t j    = t - i + 1;           /* column index (1‑based) */

    return row_value(i, j);
}
