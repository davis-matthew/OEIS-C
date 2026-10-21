/*
 * A222057  Triangle read by rows: coefficients of harmonic-geometric
 *          polynomials.
 * https://oeis.org/A222057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = S₂(r,k) * k! * H_k, where r is the unique integer
 * with r(r+1)/2 ≥ n > (r-1)r/2 and k = n - (r-1)r/2 (1‑based).
 * S₂ is Stirling numbers of the second kind, H_k = 1 + 1/2 + … + 1/k.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Random utility function prototypes are assumed to be provided elsewhere.
   We only use standard headers and basic arithmetic. */

/* External integer types supplied by the build system */
extern seq_t *seq_alloc(size_t);

/* Global caches for Stirling‑2 rows and the |Stirling1(k+1,2)| values. */
static seq_t **S_rows = NULL;      /* S_rows[r][k] = Stirling2(r,k), 0 ≤ k ≤ r */
static size_t  max_S_row = 0;      /* largest r that has been built */

static seq_t *abs_s2 = NULL;      /* abs_s2[k] = k! * H_k,  k ≥ 0 */
static size_t max_abs_s2 = 0;      /* number of values currently stored */

/* Ensure that Stirling‑2 rows up to r_needed are computed. */
static void ensure_S_rows(idx_t r_needed)
{
    if ((size_t)r_needed <= max_S_row)
        return;

    /* First time: create row 0. */
    if (S_rows == NULL) {
        S_rows = (seq_t**)calloc(1, sizeof(seq_t*));
        seq_t *row0 = (seq_t*)calloc(1, sizeof(seq_t));
        row0[0] = (seq_t)1;           /* S₂(0,0) = 1 */
        S_rows[0] = row0;
        max_S_row = 0;
    }

    for (size_t r = max_S_row + 1; r <= (size_t)r_needed; ++r) {
        size_t len = r + 1;
        seq_t *row = (seq_t*)calloc(len, sizeof(seq_t));
        row[0] = (seq_t)0;
        row[r] = (seq_t)1;
        for (size_t k = 1; k < r; ++k) {
            row[k] = (seq_t)k * S_rows[r-1][k] + S_rows[r-1][k-1];
        }
        S_rows = (seq_t**)realloc(S_rows, (r + 1) * sizeof(seq_t*));
        S_rows[r] = row;
    }
    max_S_row = (size_t)r_needed;
}

/* Ensure abs_s2[0..k_needed] = |Stirling1(k+1,2)|   (k! * H_k). */
static void ensure_abs_s2(idx_t k_needed)
{
    if ((size_t)k_needed < max_abs_s2)
        return;

    size_t new_size = (size_t)k_needed + 1;   /* store indices 0..k_needed */
    abs_s2 = (seq_t*)realloc(abs_s2, new_size * sizeof(seq_t));

    abs_s2[0] = (seq_t)0;   /* unused */
    if (new_size > 1)
        abs_s2[1] = (seq_t)1;   /* 1! * H_1 = 1 */

    seq_t fact = (seq_t)1;   /* fact = (k-1)! : start with 0! = 1 */
    for (size_t k = 2; k <= (size_t)k_needed; ++k) {
        fact = fact * (seq_t)(k - 1);   /* now fact = (k-1)! */
        abs_s2[k] = (seq_t)k * abs_s2[k-1] + fact;
    }
    max_abs_s2 = new_size;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n==0, defensive */

    /* Find the row r such that r(r+1)/2 ≥ n > (r-1)r/2. */
    idx_t r = 0;
    idx_t tri = 0;
    while (tri < n) {
        ++r;
        tri += r;          /* tri = r(r+1)/2 after the loop body */
    }
    idx_t tri_prev = tri - r;      /* sum 1…(r-1) = (r-1)r/2 */
    idx_t k = n - tri_prev;        /* position in the row, 1‑based */

    ensure_S_rows(r);
    ensure_abs_s2(k);

    seq_t value = S_rows[r][k] * abs_s2[k];
    return value;
}
