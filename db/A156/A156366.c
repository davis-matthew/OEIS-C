/*
 * A156366  Triangle of coefficients of p(n, x) where p(n, x) is defined as
 *          p(n, x) = (1-3*x)^(n+1)*PolyLog(-n, 3*x)/(3*x), read by rows.
 * https://oeis.org/A156366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,c) with r, c such that n = offset_row(r)+c, T(r,c) =
 *            3^c * Σ_{j=0}^{c} binomial(r+1,j) * (-1)^j * (c-j+1)^r
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 61, u64 n <= 162,
 *            u128 n <= 450, big any n
 * Helpers:   _floor_isqrt_u64, big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"
#include "../utils/_floor_isqrt_u64.h"

/* Return the nth term of OEIS A156366 (0‑based indexing). */
seq_t a(idx_t n)
{
    /* Handle the special first term. */
    if (n == 0)
        return (seq_t)1;

    /* Find the row r and column c for index n. */
    idx_t r, c;
    if (n == 0) {
        r = 0; c = 0;
    } else {
        /* r >= 1 */
        idx_t tmp = 1 + 8ULL * (n - 1);       /* 1 + 8*(n-1) fits in 64 bits for n <= ~2^63. */
        idx_t sqrt_tmp = _floor_isqrt_u64(tmp);
        r = (1 + sqrt_tmp) / 2;              /* floor((1+sqrt(tmp))/2) */

        idx_t offset = (r == 0) ? 0 : 1 + ((r - 1) * r) / 2;  /* start index of row r */
        c = n - offset;                      /* column within the row, 0 <= c < r */
    }

    /* The first element of every row is 1. */
    if (c == 0)
        return (seq_t)1;

    /* Compute the sum Σ_{j=0}^{c} binomial(r+1,j) * (-1)^j * (c-j+1)^r
       using separate positive and negative accumulators to avoid
       temporary underflow. */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    for (idx_t j = 0; j <= c; ++j) {
        seq_t bin = binom(r + 1, j);
        seq_t pow_term = big_pow((seq_t)(c - j + 1), (seq_t)r);
        seq_t term = bin * pow_term;
        if (j & 1)
            sum_neg += term;
        else
            sum_pos += term;
    }
    seq_t sum = sum_pos - sum_neg;             /* sum_pos >= sum_neg for all terms */

    /* Multiply by 3^c. */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)c);
    return sum * pow3;
}
