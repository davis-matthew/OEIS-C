/*
 * A092077  Generalized Stirling2 array (8,2).
 * https://oeis.org/A092077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the generalized Stirling2 array (8,2) in row-major
 *            order. Recurrence for a(n,k) (n>=2, 2<=k<=2n): a(n,k) =
 *            a(n-1,k)*((6*(n-1)+k)*(6*(n-1)+k-1)) +
 *            2*a(n-1,k-1)*(6*(n-1)+k-1) + a(n-1,k-2) Row lengths are odd:
 *            1,3,5,7,… Offset 1 (a(1)=1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 25, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stddef.h>

/* Helper to allocate an array of seq_t that works in both C and C++ */
#ifdef __cplusplus
static inline seq_t *alloc_seq_arr(size_t len) { return new seq_t[len]; }
static inline void free_seq_arr(seq_t *p)           { delete[] p; }
#else
static inline seq_t *alloc_seq_arr(size_t len) { return (seq_t *)malloc(len * sizeof(seq_t)); }
static inline void free_seq_arr(seq_t *p)           { free(p); }
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for offset < 1 */

    /* Determine the row r such that (r-1)^2 < n <= r^2  */
    idx_t r = (idx_t)floor(sqrt((long double)n));
    if (r * r < n) ++r;

    /* Allocate and initialise row 1 (length 1) */
    seq_t *prev = alloc_seq_arr(1);
    if (!prev) return (seq_t)0;             /* allocation failed */
    prev[0] = (seq_t)1;                     /* a(1,2) = 1 */

    /* Compute rows 2..r iteratively */
    for (idx_t row = 2; row <= r; ++row) {
        idx_t len   = 2 * row - 1;           /* current row length */
        seq_t *curr = alloc_seq_arr(len);
        if (!curr) { free_seq_arr(prev); return (seq_t)0; }

        seq_t base_offset = (seq_t)(6 * (row - 1));  /* 6*(n-1) */

        for (idx_t idx = 0; idx < len; ++idx) {
            idx_t k   = idx + 2;            /* column index in [2,2row] */
            seq_t val = (seq_t)0;

            /* p = 0 term */
            if (k <= 2 * (row - 1)) {
                seq_t b = base_offset + (seq_t)k;
                seq_t prod = b * (b - (seq_t)1);
                val += prev[k - 2] * prod;
            }

            /* p = 1 term */
            if (k - 1 >= 2 && k - 1 <= 2 * (row - 1)) {
                seq_t b = base_offset + (seq_t)(k - 1);
                val += (seq_t)2 * prev[(k - 1) - 2] * b;
            }

            /* p = 2 term */
            if (k - 2 >= 2 && k - 2 <= 2 * (row - 1)) {
                val += prev[(k - 2) - 2];
            }

            curr[idx] = val;
        }

        free_seq_arr(prev);   /* free previous row */
        prev = curr;          /* move to next row */
    }

    /* Index within the final row */
    idx_t prev_sq = (r - 1) * (r - 1);
    idx_t idx_in_row = n - prev_sq - 1;  /* 0‑based */

    seq_t result = prev[idx_in_row];
    free_seq_arr(prev);  /* clean up */

    return result;
}
