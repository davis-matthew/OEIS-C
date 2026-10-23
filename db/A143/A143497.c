/*
 * A143497  Triangle of unsigned 2-Lah numbers.
 * https://oeis.org/A143497
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T(n,k) = ((n - 2)!/(k - 2)!)*C(n+1, k+1) for n,k ≥ 2, T(2,2)=1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 67, u64 n <= 173,
 *            u128 n <= 498, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of unsigned 2‑Lah numbers (OEIS A143497). */
/* a(n) is the flattened row‑wise listing of this triangle with offset 2. */

#include <stdint.h>
#include <math.h>

/* Utility: integer square root (floor) for 64‑bit unsigned integers. */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x == 0 || x == 1) return x;
    long double d = sqrt((long double)x);
    idx_t r = (idx_t)d;
    /* adjust in case of rounding errors */
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Compute the n‑th term of A143497 (offset 2). */
seq_t a(idx_t n)
{
    /* Undefined for indices below the offset. */
    if (n < 2) return 0;

    /* Convert to 0‑based index inside the flattened array. */
    idx_t m = n - 2;                /* 0‑based position */

    /* Find the row number N such that
       (N-2)(N-1)/2 ≤ m < (N-1)N/2. */
    idx_t s = isqrt_u64(1 + 8 * m);
    idx_t N = (3 + s) >> 1;         /* floor division by 2 */

    /* Compute the start index of row N. */
    idx_t start = (N - 2) * (N - 1) / 2;

    /* Position within the row (0‑based). */
    idx_t offset = m - start;
    idx_t k = 2 + offset;           /* column index, 2 ≤ k ≤ N */

    /* Compute binomial coefficient C(N+1, k+1) using a multiplicative
       algorithm that keeps all intermediate values integers. */
    seq_t binom = 1;
    idx_t kb = k + 1;               /* upper index of the binomial */
    for (idx_t i = 1; i <= kb; ++i) {
        binom = binom * (seq_t)(N + 1 - (kb - i));
        binom = binom / (seq_t)i;   /* division is exact by construction */
    }

    /* Compute the factorial ratio (N-2)! / (k-2)!  as a product. */
    seq_t prod = 1;
    for (idx_t i = k - 1; i <= N - 2; ++i) {
        prod = prod * (seq_t)i;
    }

    /* Result is the product of the binomial coefficient and the factorial ratio. */
    return binom * prod;
}
