/*
 * A173917  A double product sequence based on a=2; f(n,a) = f(n-1,a) +
 *          a*f(n-2,a).
 * https://oeis.org/A173917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r,k satisfy n = r*(r+1)/2 + k, 0 ≤ k ≤ r.
 *            T(r,k) = ∏_{i=1}^{k} ((-2)^{r+1-i}-1)/((-2)^{i}-1) *
 *            ((-2)^{r+2-i}-1)/((-2)^{i+1}-1)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 38, u64 n <= 81, u128 n <= 142,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"
/* ------------- utility helpers ------------- */
static inline seq_t
pow2(idx_t exp)
{
    seq_t p = 1;
    /* multiply by 2 exp times; works for all seq_t types. */
    for (idx_t i = 0; i < exp; ++i)
        p *= 2;
    return p;
}

/* absolute value of (-2)^exp – 1  (exp ≥ 1) */
static inline seq_t
abs_powminus1(idx_t exp)
{
    seq_t p = pow2(exp);          /* 2^exp */
    if (exp & 1U)                 /* odd exponent → -2^exp - 1 → |-2^exp -1| = 2^exp + 1 */
        return p + 1;
    else                          /* even exponent → 2^exp - 1 */
        return p - 1;
}

/* greatest common divisor for seq_t (Euclid's algorithm) */

/* ------------- main triangle element ------------- */
static inline seq_t
compute_T(idx_t n, idx_t k)
{
    if (k == 0)
        return (seq_t)1;

    seq_t numer = 1, denom = 1;

    for (idx_t i = 1; i <= k; ++i) {
        /* exponents for the four terms */
        idx_t exp_num1 = n + 1 - i;   /* q^(n+1-i)  */
        idx_t exp_den1 = i;           /* q^i        */
        idx_t exp_num2 = n + 2 - i;   /* q^(n+2-i)  */
        idx_t exp_den2 = i + 1;       /* q^(i+1)    */

        seq_t a_num = abs_powminus1(exp_num1);
        seq_t a_den = abs_powminus1(exp_den1);
        seq_t b_num = abs_powminus1(exp_num2);
        seq_t b_den = abs_powminus1(exp_den2);

        /* reduce a_num against current denominator */
        seq_t g = gcd_seq(a_num, denom);
        if (g > 1) {
            a_num /= g;
            denom /= g;
        }

        /* reduce b_num against current denominator */
        g = gcd_seq(b_num, denom);
        if (g > 1) {
            b_num /= g;
            denom /= g;
        }

        /* reduce a_den against current numerator */
        g = gcd_seq(a_den, numer);
        if (g > 1) {
            a_den /= g;
            numer /= g;
        }

        /* reduce b_den against current numerator */
        g = gcd_seq(b_den, numer);
        if (g > 1) {
            b_den /= g;
            numer /= g;
        }

        numer *= a_num;
        numer *= b_num;
        denom *= a_den;
        denom *= b_den;

        /* final quick reduction to keep numbers small */
        g = gcd_seq(numer, denom);
        if (g > 1) {
            numer /= g;
            denom /= g;
        }
    }

    /* denom should now divide numer exactly */
    return numer / denom;
}

/* ------------- index → (r,k) and element ------------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* find the largest r such that r*(r+1)/2 ≤ n  (binary search) */
    idx_t low = 0, high = n;
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    idx_t k = n - r * (r + 1) / 2;   /* offset within row r */
    return compute_T(r, k);
}
