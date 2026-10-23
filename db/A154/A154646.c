/*
 * A154646  Triangle T(n,k) with the coefficient [x^k] of the series
 *          (1-x)^(n+1) * Sum_{m>=0} ((3*m+1)^n + (3*m+2)^n)*x^m in row n,
 *          column k.
 * https://oeis.org/A154646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1) – 1)/2⌋, k = n – r(r+1)/2, and
 *            T(r,k) = Σ_{t=0}^{k} ((3(k‑t)+1)^r + (3(k‑t)+2)^r) * (–1)^t *
 *            C(r+1,t). This evaluates the coefficient [x^k] of (1–x)^(r+1) ·
 *            Σ_{m≥0} ((3m+1)^r + (3m+2)^r) x^m
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned integer, using binary search.
   Returns ⌊√v⌋.  Uses 128‑bit arithmetic to avoid overflow. */
static idx_t isqrt_u64(idx_t v)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* Expand high until high*high > v */
    while ((__uint128_t)high * high <= v) {
        high <<= 1;
    }
    /* Binary search in [low, high) */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if ((__uint128_t)mid * mid <= v) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

/* Power function: base^exp written in seq_t.  Uses exponentiation by squaring. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    /* --- Determine row (r) and column (k) from the flattened index n --- */
    idx_t r = (isqrt_u64((idx_t)((__uint128_t)8 * n + 1)) - 1) / 2;
    idx_t k = n - (r * (r + 1)) / 2;

    /* --- Compute the triangle entry T(r,k) --- */
    seq_t result = 0;
    seq_t binom = 1;                 /* C(r+1, 0) = 1 */

    for (idx_t t = 0; t <= k; ++t) {
        idx_t m = k - t;
        seq_t base1 = 3 * m + 1;
        seq_t base2 = 3 * m + 2;

        seq_t sum_pow = pow_seq(base1, r) + pow_seq(base2, r);
        seq_t term    = binom * sum_pow;

        /* Add or subtract depending on the parity of t. */
        if (t & 1)
            result -= term;
        else
            result += term;

        /* Update binomial coefficient to C(r+1, t+1) for next iteration. */
        if (t < k) {
            binom = binom * (r + 1 - t) / (t + 1);
        }
    }

    return result;
}
