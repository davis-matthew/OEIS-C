/*
 * A124844  Triangle T(n,k)=binomial(n,k)*A061084(k), 0<=k<=n, read by rows.
 * https://oeis.org/A124844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = binomial(m, k) * c_k where (m, k) satisfy n = m(m+1)/2 +
 *            k (0 ≤ k ≤ m) c_0 = 1 for k ≥ 1: c_k = (-1)^(k+1) * L_{k-1} L_n
 *            is the n‑th Lucas number with L_0 = 2, L_1 = 1, L_{n+2} =
 *            L_{n+1} + L_n (Lucas numbers: 2,1,3,4,7,11,18,29,…) Thus |c_k| =
 *            Lucas_{k-1} and signs alternate starting with...
 * Range:     i8 n <= 31, i16 n <= 98, i32 n <= 338, i64 n <= 1250,
 *            i128 n <= 4614, big any n
 * Helpers:   _floor_isqrt_u64, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* utility functions provided by the build system */
#include "../utils/binom.h"
#include "../utils/_floor_isqrt_u64.h"

/* compute Lucas number L_n (n >= 0) */
static seq_t lucas_number(idx_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 1;
    seq_t a0 = 2;
    seq_t a1 = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = a0 + a1;
        a0 = a1;
        a1 = tmp;
    }
    return a1;
}

seq_t a(idx_t n)
{
    /* find row index m: m = floor((sqrt(8*n+1)-1)/2) */
    idx_t sq = _floor_isqrt_u64(8*n + 1);   /* sqrt is small for n <= 10000 */
    idx_t m = (sq - 1) / 2;

    /* column index k */
    seq_t row_start = ((seq_t)m * ((seq_t)m + 1)) / 2;
    idx_t k = n - (idx_t)row_start;

    /* diagonal coefficient c_k */
    seq_t c;
    if (k == 0) {
        c = 1;
    } else {
        seq_t abs_c = lucas_number(k - 1);    /* |c_k| = Lucas_{k-1} */
        /* sign: positive for odd k, negative for even k (k >= 1) */
        if (k % 2 == 0) {   /* even k -> negative */
            c = -abs_c;
        } else {            /* odd k -> positive */
            c = abs_c;
        }
    }

    /* binomial coefficient */
    seq_t bin = binom(m, k);

    return bin * c;
}
