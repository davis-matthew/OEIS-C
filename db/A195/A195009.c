/*
 * A195009  Triangle read by rows, T(n,k) = k^n*A056040(n), n>=0, 0<=k<=n.
 * https://oeis.org/A195009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k^n * A056040(row), where * row, k are obtained from the
 *            flattened index n by * row = floor((sqrt(8*n+1)-1)/2) and k = n
 *            - row*(row+1)/2 * A056040(row) = row! / (floor(row/2)!^2) * =
 *            C(2m, m) if row = 2m * = (2m+1) * C(2m, m) if row = 2m+1 *
 *            (central binomial coefficient) * * The function...
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binom_n_k, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/binom_n_k.h"

/* Static cache to avoid recomputing factorials / binomials */
static idx_t fact_cache_size = 0;

/* Main sequence function */
seq_t a(idx_t idx)
{
    /* Compute the row index r such that
     * offset = r*(r+1)/2 <= idx < (r+1)*(r+2)/2   */
    idx_t r = (_floor_isqrt_u64(8 * idx + 1) - 1) / 2;

    /* Offset of the first element of row r */
    idx_t offset = r * (r + 1) / 2;

    /* Column position within the row */
    idx_t k = idx - offset;

    /* Handle the trivial first element 0^0 = 1, A(0)=1 */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Compute A(row) = row! / (floor(row/2)!^2 )
     *                     = binom(2m, m)   if row = 2m
     *                     = (2m+1)*binom(2m, m) if row = 2m+1
     */
    idx_t m = r / 2;                /* floor(r/2) */
    seq_t binom_coeff = binom_n_k(2 * m, m);

    seq_t A;
    if ((r & 1) == 0) { /* even row */
        A = binom_coeff;
    } else {            /* odd row */
        A = binom_coeff * (seq_t)(r);   /* r = 2m+1 */
    }

    /* Compute k^row */
    seq_t pow_k;
    if (k == 0 && r > 0) {
        pow_k = (seq_t)0;                /* 0^r with r>0 */
    } else {
        pow_k = seq_pow((seq_t)k, r);
    }

    /* Result: k^row * A */
    return pow_k * A;
}
