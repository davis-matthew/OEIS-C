/*
 * A115068  Triangle read by rows: T(n,k) = number of elements in the Coxeter
 *          group D_n with descent set contained in {s_k}, for 0<=k<=n-1.
 * https://oeis.org/A115068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(r, k) * 2^(r-k-1) , where r is the smallest
 *            integer with r(r+1)/2 ≥ n, and k = n - (r-1)r/2 - 1 (0‑based
 *            column in the triangle)
 * Range:     u8 n <= 23, u16 n <= 80, u32 n <= 258, u64 n <= 916,
 *            u128 n <= 3510, big any n
 * Helpers:   binomial, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the triangle row r such that n lies in it. */
    idx_t r = 0;
    while (((unsigned __int128)r * (r + 1)) / 2 < n)
        ++r;

    /* Compute column index k (0‑based). */
    idx_t prev_row_count = ((unsigned __int128)(r - 1) * r) / 2;
    idx_t k = n - prev_row_count - 1;   /* k ∈ [0, r-1] */

    /* Compute T(r, k) = binomial(r, k) * 2^(r-k-1). */
    seq_t bin = binomial(r, k);
    seq_t two_pow = pow2(r - k - 1);
    seq_t result = seq_mul(bin, two_pow);

    return result;
}
