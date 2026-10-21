/*
 * A098493  Triangle T(n,k) read by rows: difference between A098489 and
 *          A098490 at triangular rows.
 * https://oeis.org/A098493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the flattened triangle T(n,k) read by rows, where T(n,k)
 *            satisfies T(n,n) = (-1)^n, T(n,n-1) = (-1)^n * (1-n), T(n,k) =
 *            T(n-1,k) - T(n-1,k-1) - T(n-2,k) (for 0 ≤ k < n-1), with base
 *            rows T(0,0) = 1, T(1,0) = 0, T(1,1) = -1
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit values. */
static inline uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = x, mid;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;   /* protect against overflow */
        if (sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Compute the OEIS sequence A098493 */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    uint64_t t = (uint64_t)8 * n + 1;
    uint64_t sq = floor_sqrt_u64(t);
    uint64_t r = (sq - 1) / 2;
    uint64_t triangular = r * (r + 1) / 2;
    uint64_t k = n - triangular;   /* column index */

    if (r == 0) {
        return (seq_t)1;   /* T(0,0) */
    }
    if (r == 1) {
        return (k == 0) ? (seq_t)0 : (seq_t)(-1);   /* T(1,0)=0, T(1,1)=-1 */
    }

    /* Allocate three rows of size r+1 (extra space for indices up to r) */
    size_t sz = (size_t)(r + 1);
    seq_t *prev2 = (seq_t *)calloc(sz, sizeof(seq_t));  /* row i-2 */
    seq_t *prev1 = (seq_t *)calloc(sz, sizeof(seq_t));  /* row i-1 */
    seq_t *curr  = (seq_t *)calloc(sz, sizeof(seq_t));  /* row i  */

    /* Initialize row 0 and row 1 */
    prev2[0] = (seq_t)1;           /* T(0,0) */
    prev1[0] = (seq_t)0;           /* T(1,0) */
    prev1[1] = (seq_t)(-1);        /* T(1,1) */

    /* Build rows 2 .. r */
    for (uint64_t i = 2; i <= r; ++i) {
        for (uint64_t j = 0; j <= i; ++j) {
            if (j == i) {
                curr[j] = (i & 1) ? (seq_t)(-1) : (seq_t)1;
            } else if (j == i - 1) {
                curr[j] = (i & 1) ? (seq_t)(i - 1) : (seq_t)(1 - (seq_t)i);
            } else {
                seq_t a = prev1[j];
                seq_t b = (j > 0) ? prev1[j - 1] : 0;
                seq_t c = prev2[j];
                curr[j] = a - b - c;
            }
        }
        /* Rotate rows: prev2 <- prev1, prev1 <- curr, curr reused */
        seq_t *tmp   = prev2;
        prev2 = prev1;
        prev1 = curr;
        curr  = tmp;
    }

    /* prev1 now holds row r; answer is entry k */
    seq_t result = prev1[k];

    /* Free allocated memory */
    free(prev2);
    free(prev1);
    free(curr);

    return result;
}
