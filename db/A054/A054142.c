/*
 * A054142  Triangular array binomial(2*n-k, k), k=0..n, n >= 0.
 * https://oeis.org/A054142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑major flattening of the triangle T(n,k) =
 *            binomial(2*n-k, k) for 0 ≤ k ≤ n. For a given index n, determine
 *            the row r and column k, then return binomial(2*r - k, k)
 * Range:     u8 n <= 7, u16 n <= 99, u32 n <= 338, u64 n <= 495,
 *            u128 n <= 495, big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Compute the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
       Use the integer solution of r = ⌊(√(8n+1)-1)/2⌋. */
    idx_t m = 8 * n + 1;
    seq_t sqrt_m = floor_sqrt((seq_t)m);
    idx_t r = ((idx_t)sqrt_m - 1) / 2;

    /* Ensure r is not too large due to floor rounding. */
    if (r * (r + 1) / 2 > n) {
        r--;
    } else if ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    idx_t row_start = r * (r + 1) / 2; /* first index of this row */
    idx_t k = n - row_start;          /* column within the row */

    /* Compute the value using the provided binomial helper. */
    return binomial(2 * r - k, k);
}
