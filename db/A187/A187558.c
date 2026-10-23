/*
 * A187558  Triangle a(n,k) read by rows: product s(n,k)*s(n+1,k+1) of
 *          Stirling numbers of the first kind.
 * https://oeis.org/A187558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 92, u128 n <= 231,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = s(n,k) * s(n+1,k+1) where
 *   k = n - r(r+1)/2
 *   r = floor((sqrt(8n+1)-1)/2)
 *   s(m,ℓ) is the unsigned Stirling number of the 1st kind.
 *
 * The function computes the flattened triangle entry a(n) by
 * determining the row r and column k and evaluating the
 * corresponding Stirling numbers via the recurrence
 *
 *     s(0,0)=1
 *     s(m,ℓ) = s(m-1,ℓ-1) + (m-1) * s(m-1,ℓ)  (m>0, 1≤ℓ≤m)
 *
 * Only 64‑bit integer arithmetic is used; the result is returned
 * in the build‑dependent type seq_t.  No allocation is performed
 * across calls, and the computation is O(r²) with r≈√(2n),
 * which is well below the 2‑second limit for n≈10⁴.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root of a 64‑bit unsigned number. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1ULL << 32; /* hi^2 >= x for all 64‑bit x */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Determine row r and column k within that row. */
    idx_t rad = isqrt_u64(8 * n + 1);
    idx_t r   = (rad - 1) / 2;                    /* floor((sqrt(8n+1)-1)/2) */
    idx_t prev = r * (r + 1) / 2;                 /* cumulative terms before row r */
    idx_t k   = n - prev;                         /* position within the row, 0 ≤ k ≤ r */

    /* We need Stirling numbers up to row r+1. */
    idx_t need = r + 1;

    /* Allocate temporary rows for DP. */
    seq_t *prev_row = (seq_t*)malloc((need + 1) * sizeof(seq_t));
    seq_t *curr_row = (seq_t*)malloc((need + 1) * sizeof(seq_t));
    if (!prev_row || !curr_row) {
        /* Allocation failed; return 0 as a safe fallback. */
        free(prev_row);
        free(curr_row);
        return (seq_t)0;
    }

    /* Initialize row 0. */
    for (idx_t i = 0; i <= need; ++i) prev_row[i] = (seq_t)0;
    prev_row[0] = (seq_t)1;                     /* s(0,0)=1 */

    seq_t val1 = (seq_t)1;                       /* s(0,0) when r==0 */
    seq_t val2 = (seq_t)0;                       /* will be set when i==r+1 */
    if (r == 0) val1 = prev_row[0];

    /* Build rows 1 through need. */
    for (idx_t i = 1; i <= need; ++i) {
        curr_row[0] = (seq_t)0;
        for (idx_t j = 1; j <= i; ++j) {
            curr_row[j] = prev_row[j - 1] + (seq_t)(i - 1) * prev_row[j];
        }

        if (i == r) {
            val1 = curr_row[k];                 /* s(r,k)   */
        }
        if (i == r + 1) {
            val2 = curr_row[k + 1];             /* s(r+1,k+1) */
        }

        /* Copy current row to prev_row for next iteration. */
        for (idx_t j = 0; j <= i; ++j) prev_row[j] = curr_row[j];
    }

    /* Free temporary memory. */
    free(prev_row);
    free(curr_row);

    /* Return the product. */
    return val1 * val2;
}
