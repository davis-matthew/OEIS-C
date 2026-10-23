/*
 * A168525  Triangle of coefficients of g.f. a*(1+x)^n +
 *          b*(1-x)^(n+2)*polylog(-n-1, x)/x + 2^n*c*(1-x)^(n+1)*LerchPhi(x,
 *          -n, 1/2), with a = 65/2, b = -162/2, c = 135/2.
 * https://oeis.org/A168525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 48, u64 n <= 142, u128 n <= 416,
 *            big any n
 * Helpers:   _col_from_index, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A168525
 *
 *  The triangle T(r,k) is given by the coefficient of x^k in
 *      (65/2)*(1+x)^r
 *    + (-162/2)*(1-x)^(r+2)*polylog(-r-1, x)/x
 *    + 2^r*(135/2)*(1-x)^(r+1)*LerchPhi(x, -r, 1/2)
 *
 *  with a = 65/2, b = -162/2, c = 135/2.
 *
 *  The nth term of the flattened row‑major triangle is returned.
 *
 *  Formula used (derived from the generating function):
 *
 *      T(r,k) = (65*C(r,k)+135*S)/(2) - 81*U
 *
 *      S = Σ_{t=0}^{min(k,r+1)} (-1)^t * C(r+1,t) * (2(k‑t)+1)^r
 *      U = Σ_{t=0}^{min(k,r+2)} (-1)^t * C(r+2,t) * (k‑t+1)^(r+1)
 *
 *  Row r is given by r = ⌊½(⌊√(8n+1)⌋‑1)⌋,
 *  column k = n – r(r+1)/2.  The implementation computes r by an
 *  integer binary search to avoid floating‑point operations.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/_col_from_index.h"

/* ---- helper: row from flattened index --------------------------------- */
static inline idx_t _row_from_index(idx_t n)
{
    /* find largest r with r*(r+1)/2 <= n */
    idx_t low = 0, high = 1;
    while ((high * (high + 1) / 2) <= n)
        high <<= 1;            /* increase range exponentially */

    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if ((mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* ---- helper: multiplicative binomial C(n,k) --------------------------- */
static inline seq_t _binom_n_k(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* ---- main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    /* locate (row r, column k) */
    idx_t r = _row_from_index(n);
    idx_t k = _col_from_index(n, r);

    /* binomial C(r,k) */
    seq_t binom_r_k = _binom_n_k(r, k);

    /* coefficient S_{r,k} */
    seq_t S = 0;
    seq_t binom_sp = 1;       /* C(r+1,0) */
    int sign = 1;             /* (-1)^t */
    idx_t max_tS = k < r + 1 ? k : r + 1;
    for (idx_t t = 0; t <= max_tS; ++t) {
        idx_t m = k - t;                     /* m = k-t */
        seq_t base = 2 * m + 1;              /* integer base */
        seq_t pwr = big_pow((seq_t)base, (seq_t)r);
        seq_t term = binom_sp * pwr;
        if (sign > 0) S += term; else S -= term;
        /* update binomial for next t */
        if (t < max_tS) {
            binom_sp = binom_sp * (r + 1 - t) / (t + 1);
        }
        sign = -sign;
    }

    /* coefficient U_{r,k} */
    seq_t U = 0;
    seq_t binom_up = 1;       /* C(r+2,0) */
    sign = 1;
    idx_t max_tU = k < r + 2 ? k : r + 2;
    for (idx_t t = 0; t <= max_tU; ++t) {
        idx_t p = k - t + 1;               /* base for power */
        seq_t pwr = big_pow((seq_t)p, (seq_t)(r + 1));
        seq_t term = binom_up * pwr;
        if (sign > 0) U += term; else U -= term;
        /* update binomial for next t */
        if (t < max_tU) {
            binom_up = binom_up * (r + 2 - t) / (t + 1);
        }
        sign = -sign;
    }

    /* final value */
    seq_t num = 65 * binom_r_k + 135 * S;
    seq_t part = num / 2;          /* exact division */
    seq_t result = part - 81 * U;
    return result;
}
