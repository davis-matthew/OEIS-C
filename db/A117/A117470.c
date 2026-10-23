/*
 * A117470  Triangle read by rows: T(n,k) is the number of partitions of n in
 *          which every integer from the smallest part to the largest part
 *          occurs and the difference between the largest part and the
 *          smallest part is k (n >= 0, k >= 0).
 * https://oeis.org/A117470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117470 – Triangle read by rows:
 * T(n,k) is the number of partitions of n in which every integer from
 * the smallest part to the largest part occurs and the difference between
 * the largest and smallest part is k.  The function returns the sequence
 * read row‑by‑row,  a(1) = first term, a(2) = second term, etc.  */
  
#include <stdlib.h>
#include <stdint.h>

/* ----------------------------------------------------------------------
 * Helper: floor(sqrt(n)) for n <= 2^64-1.  Uses 128‑bit arithmetic to avoid
 * overflow; the result fits in 32 bits because sqrt(2^64-1) < 2^32.
 * ---------------------------------------------------------------------- */
static inline idx_t isqrt64(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1 << 32) - 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) low = mid;
        else          high = mid - 1;
    }
    return low;
}

/* ----------------------------------------------------------------------
 * row_length(n) = ceil((sqrt(9+8*n)-3)/2);  returns an idx_t.
 * ---------------------------------------------------------------------- */
static inline idx_t row_length(idx_t n)
{
    idx_t val = 9 + 8 * n;                    /* 9+8n fits in 64‑bit */
    idx_t s   = isqrt64(val);                 /* floor(sqrt(9+8n)) */
    if (s * s < val) ++s;                     /* ceil(sqrt)               */
    return (s - 3 + 1) / 2;                   /* ceil((s-3)/2)             */
}

/* ----------------------------------------------------------------------
 * Global state: flattened triangle values and row offsets.
 * ---------------------------------------------------------------------- */
static seq_t *seq_vals = NULL;    /* flattened triangle, 0‑based index */
static idx_t *row_off  = NULL;    /* row_off[n] = start index of row n */
static idx_t   precomp   = 0;     /* maximum n that has been pre‑computed */

/* ----------------------------------------------------------------------
 * Rebuild the tables up to new_max (inclusive).
 * ---------------------------------------------------------------------- */
static void rebuild_tables(idx_t new_max)
{
    /* free old tables if they exist */
    free(seq_vals);
    free(row_off);

    /* compute row offsets */
    row_off = (idx_t *)malloc((new_max + 2) * sizeof(idx_t));
    row_off[1] = 0;
    for (idx_t n = 2; n <= new_max; ++n)
        row_off[n] = row_off[n-1] + row_length(n-1);

    /* total number of entries in the flattened triangle */
    idx_t total = row_off[new_max] + row_length(new_max);

    seq_vals = (seq_t *)calloc(total, sizeof(seq_t));   /* zero‑init */

    /* temporary DP array for a fixed smallest part m */
    seq_t *dp = (seq_t *)calloc((new_max + 1), sizeof(seq_t));

    for (idx_t m = 1; m <= new_max; ++m) {
        /* reset dp: dp[0] = 1, others 0 */
        for (idx_t i = 0; i <= new_max; ++i)
            dp[i] = 0;
        dp[0] = 1;

        for (idx_t k = 0; ; ++k) {
            idx_t coin = m + k;
            if (coin > new_max) break;          /* no further coins */

            /* update dp with this coin */
            for (idx_t v = coin; v <= new_max; ++v)
                dp[v] += dp[v - coin];

            /* minimal sum with one copy of each part */
            idx_t k_plus = k + 1;
            idx_t S_min = k_plus * m + (k * k_plus) / 2;
            if (S_min > new_max) break;         /* cannot reach larger n */

            idx_t maxRest = new_max - S_min;
            for (idx_t r = 0; r <= maxRest; ++r) {
                idx_t n_val = S_min + r;        /* actual n */
                idx_t pos   = row_off[n_val] + k;/* linear index */

                /* guard against out of range k for this row */
                if (k < row_length(n_val))
                    seq_vals[pos] += dp[r];
            }
        }
    }

    free(dp);
    precomp = new_max;
}

/* ----------------------------------------------------------------------
 * Public accessor: returns a(n) with offset 1.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset <1 */

    /* ensure tables are pre‑computed up to requested n */
    if (precomp < n)
        rebuild_tables(n);

    /* a(n) is located at linear index n-1 in seq_vals */
    return seq_vals[n-1];
}
