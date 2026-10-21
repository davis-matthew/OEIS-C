/*
 * A057964  Triangle T(n,k) of number of minimal 3-covers of a labeled n-set
 *          that cover k points of that set uniquely (k=3,..,n).
 * https://oeis.org/A057964
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = C(r,k) * S(k,3) * 4^(r-k), r =
 *            minimal integer with (r-2)(r-1)/2 ≥ n-2, k = 3 + (n-2 -
 *            (r-3)(r-2)/2). S(k,3) = (3^k - 3·2^k + 3)/6
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *pow2_arr = NULL;
static seq_t *pow3_arr = NULL;
static seq_t *pow4_arr = NULL;
static idx_t pow_cap  = 0;

/* Allocate and set up power tables up to size max_n (inclusive). */
static void ensure_pow_arrays(idx_t max_n)
{
    if (max_n <= pow_cap) return;

    seq_t *new2 = (seq_t*)malloc((max_n + 1) * sizeof(seq_t));
    seq_t *new3 = (seq_t*)malloc((max_n + 1) * sizeof(seq_t));
    seq_t *new4 = (seq_t*)malloc((max_n + 1) * sizeof(seq_t));

    new2[0] = new3[0] = new4[0] = (seq_t)1;
    for (idx_t i = 1; i <= max_n; ++i) {
        new2[i] = new2[i-1] * (seq_t)2;
        new3[i] = new3[i-1] * (seq_t)3;
        new4[i] = new4[i-1] * (seq_t)4;
    }

    /* free old tables if any */
    if (pow2_arr) free(pow2_arr);
    if (pow3_arr) free(pow3_arr);
    if (pow4_arr) free(pow4_arr);

    pow2_arr = new2;
    pow3_arr = new3;
    pow4_arr = new4;
    pow_cap   = max_n;
}

/* Find minimal r such that (r-2)(r-1)/2 ≥ i (1‑based). */
static idx_t row_for_index(idx_t i)
{
    idx_t low  = 2, high = 2;
    while ((high-2)*(high-1)/2 < i) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((mid-2)*(mid-1)/2) >= i) high = mid;
        else                          low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* i = position of the desired entry in the flattened triangle */
    idx_t i = n - 2;                    /* 1‑based index */
    idx_t r = row_for_index(i);         /* row number */
    idx_t cum_before = (r-3)*(r-2)/2;   /* entries before this row */
    idx_t offset = i - cum_before - 1;  /* 0‑based offset in this row */
    idx_t k = 3 + offset;               /* column (unique‐covered count) */

    /* ensure tables large enough for r and k */
    idx_t max_needed = r;
    if (k > max_needed) max_needed = k;
    ensure_pow_arrays(max_needed);

    /* Binomial coefficient C(r,k) */
    seq_t binom = (seq_t)1;
    for (idx_t j = 1; j <= k; ++j) {
        binom = binom * (seq_t)(r - j + 1) / (seq_t)j;
    }

    /* Stirling number S(k,3) = (3^k - 3·2^k + 3)/6 */
    seq_t temp = pow3_arr[k] - (seq_t)3 * pow2_arr[k] + (seq_t)3;
    seq_t S    = temp / (seq_t)6;       /* exact division */

    /* 4^(r-k) */
    seq_t pow4 = pow4_arr[r - k];

    return binom * S * pow4;
}
