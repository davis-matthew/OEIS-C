/*
 * A157640  Triangle of the elementwise product of binomial coefficients with
 *          q-binomial coefficients [n,k] for q = 3.
 * https://oeis.org/A157640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened cell value T(r,k) of the triangle defined
 *            by T(r,k) = binomial(r,k) * A022167(r,k) for q = 3. r =
 *            floor((sqrt(8*n+1)-1)/2) (row index) k = n - r*(r+1)/2 (column
 *            index in that row)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 39, u64 n <= 82, u128 n <= 160,
 *            big any n
 * Helpers:   isqrt_u64, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isqrt_u64.h"
#include "../utils/pow3.h"

/* Compute the n-th term of OEIS sequence A157640 */
seq_t a(idx_t n)
{
    /* Determine row r and column k from the linear index n */
    uint64_t d = 8ULL * (uint64_t)n + 1ULL;
    uint64_t s = isqrt_u64(d);
    idx_t r = (idx_t)(s - 1) / 2;
    idx_t prev = r * (r + 1) / 2;          /* terms before row r */
    idx_t k = n - prev;                    /* column index (0‑based) */

    /* Compute binomial coefficient binom(r,k) multiplicatively */
    seq_t bin = 1;
    if (k != 0) {
        for (idx_t i = 1; i <= k; ++i) {
            bin *= (seq_t)(r - k + i);     /* numerator part */
            bin /= (seq_t)i;               /* divide by denominator */
        }
    }

    /* Compute the Gaussian (q‑binomial) coefficient [r choose k]_3 */
    seq_t qbin = 1;
    if (k != 0) {
        for (idx_t i = 0; i < k; ++i) {
            seq_t numer = pow3(r - i) - 1; /* 3^(r-i) – 1 */
            seq_t denom = pow3(i + 1) - 1; /* 3^(i+1) – 1 */
            qbin *= numer;
            qbin /= denom;
        }
    }

    return bin * qbin;
}
