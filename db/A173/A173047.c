/*
 * A173047  Triangle T(n, k, q) = binomial(n, k) + q^n*binomial(n-2, k-1) - 1
 *          with T(n, 0) = T(n, n) = 1 and q = 3, read by rows.
 * https://oeis.org/A173047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k,3) where r = ⌊(√(8·n + 1) – 1)/2⌋ and k = n – r·(r
 *            + 1)/2. For k = 0 or k = r the value is 1. Otherwise a(n) =
 *            binomial(r, k) + 3^r · binomial(r – 2, k – 1) – 1
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 111, u64 n <= 388,
 *            u128 n <= 1399, big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow3.h"

/* Integer square root of a 128‑bit value: floor(sqrt(x)). */
static inline uint64_t floor_sqrt_u128(__uint128_t x)
{
    uint64_t low = 0, high = (uint64_t)1 << 33;   /* sqrt(2^66) < 2^33 */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    __uint128_t val = (__uint128_t)8 * n + 1;
    uint64_t sqrt_val = floor_sqrt_u128(val);
    idx_t r = (idx_t)((sqrt_val - 1) / 2);      /* row index          */

    /* Position k within the row. */
    idx_t cum_before = r * (r + 1) / 2;         /* terms before row r */
    idx_t k = n - cum_before;                  /* 0 <= k <= r       */

    if (k == 0 || k == r)                      /* Boundary entries  */
        return (seq_t)1;

    /* Compute the triangle entry T(r,k,3). */
    seq_t bin_n_k   = binomial(r, k);          /* binomial(r, k)     */
    seq_t bin_n2_k1 = binomial(r - 2, k - 1);  /* binomial(r-2, k-1) */
    seq_t pow3_r    = pow3(r);                  /* 3^r                */
    return bin_n_k + pow3_r * bin_n2_k1 - 1;    /* T(r,k,3)            */
}
