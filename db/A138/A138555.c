/*
 * A138555  Indices where A138554 requires only squares < floor(sqrt(n))^2.
 * https://oeis.org/A138555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138555.
   a(n) = the n-th integer k (k ≥ 1) such that every representation of k
   as a sum of squares of integers that minimises the sum of the square‑
   roots uses only integers strictly smaller than ⌊√k⌋.
   Offset: 1 (a(1) = 32). */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root: floor(sqrt(n)) for uint64_t n. */
static idx_t floor_sqrt_idx(idx_t n)
{
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* -------------------------------------------------------------------- */
/*  Dynamic‑programming tables for A138554 (minimal sum of square roots) */
/* -------------------------------------------------------------------- */
static seq_t *dp           = NULL;   /* dp[0..last] = minimal sum of ks */
static size_t dp_cap       = 0;
static idx_t  dp_last      = 0;       /* highest n computed */

/*  List of indices belonging to A138555.               */
static idx_t *indices      = NULL;   /* array of n where property holds */
static size_t idx_cap      = 0;
static size_t idx_len      = 0;

/* Ensure dp array has room for index `need`.  */
static void ensure_dp_capacity(idx_t need)
{
    if (dp_cap <= (size_t)need) {
        size_t new_cap = dp_cap ? dp_cap << 1 : 16;
        while (new_cap <= (size_t)need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        dp = tmp;
        dp_cap = new_cap;
    }
}

/* Ensure indices array has room for `need` elements.  */
static void ensure_idx_capacity(size_t need)
{
    if (idx_cap <= need) {
        size_t new_cap = idx_cap ? idx_cap << 1 : 16;
        while (new_cap <= need) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(indices, new_cap * sizeof(idx_t));
        indices = tmp;
        idx_cap = new_cap;
    }
}

/* Compute dp[dp_last+1 .. target] and update indices list.  */
static void compute_up_to(idx_t target)
{
    if (dp == NULL) {
        dp   = (seq_t *)malloc(16 * sizeof(seq_t));
        dp_cap = 16;
        dp[0] = 0;
        dp_last = 0;
    }
    if (indices == NULL) {
        indices = (idx_t *)malloc(16 * sizeof(idx_t));
        idx_cap = 16;
        idx_len = 0;
    }

    for (idx_t n = dp_last + 1; n <= target; ++n) {
        ensure_dp_capacity(n);

        /* m = ⌊√n⌋ */
        idx_t m = floor_sqrt_idx(n);

        /* Compute minimal sum of ks for n.
           Initialise with the first candidate k = 1, which always exists. */
        seq_t best = dp[n - 1] + (seq_t)1;
        for (idx_t k = 2; k <= m; ++k) {
            idx_t sq = k * k;
            seq_t cand = dp[n - sq] + (seq_t)k;
            if (cand < best) best = cand;
        }
        dp[n] = best;

        /* Check whether a minimal representation uses the square m^2. */
        seq_t candM = dp[n - m * m] + (seq_t)m;
        if (candM != best) {        /* no minimal sum uses m^2 */
            ensure_idx_capacity(idx_len + 1);
            indices[idx_len++] = n;
        }
    }
    dp_last = target;
}

/* -------------------------------------------------------------------- */
/*  Public interface                                                   */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Compute indices until we have at least n elements. */
    while (idx_len < (size_t)n) {
        idx_t next = dp_last ? dp_last + 1 : 1;
        compute_up_to(next);
    }

    return (seq_t)indices[n - 1];
}
