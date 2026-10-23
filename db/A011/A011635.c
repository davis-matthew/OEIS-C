/*
 * A011635  42nd cyclotomic polynomial.
 * https://oeis.org/A011635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in Φ_42(x), a(0) … a(12) are the
 *            coefficients of the 42nd cyclotomic polynomial. The coefficients
 *            are computed from the definition Φ_n(x) = (x^n − 1) / ∏_{d|n,
 *            d<n} Φ_d(x)
 * Range:     i8 n <= 12, i16 n <= 12, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute Φ_d(x) for all divisors d of 42 recursively and cache the
   coefficients.  The maximum degree is 12 (φ(42)).  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper: multiply two polynomials a and b where a has degree deg_a,
   b has degree deg_b.  Result is stored in out (length deg_a+deg_b+1).  */
static void poly_mul(const seq_t *a, size_t deg_a,
                     const seq_t *b, size_t deg_b,
                     seq_t *out)
{
    size_t i, j;
    /* initialize to zero */
    for (i = 0; i <= deg_a + deg_b; ++i) out[i] = 0;
    for (i = 0; i <= deg_a; ++i) {
        if (a[i] == 0) continue;
        for (j = 0; j <= deg_b; ++j) {
            out[i + j] += a[i] * b[j];
        }
    }
}

/* Helper: long division of divisor (deg_d) into dividend (deg_n).
   The division is known to be exact; leading coefficient of divisor
   is 1.  Quotient occupies quotient array of length deg_n - deg_d + 1. */
static void poly_div(const seq_t *dividend, size_t deg_n,
                     const seq_t *divisor, size_t deg_d,
                     seq_t *quotient)
{
    size_t i;
    seq_t rem[43]; /* n <= 42, so degree <= 42 */
    for (i = 0; i <= deg_n; ++i) rem[i] = dividend[i];
    for (i = 0; i <= deg_n - deg_d; ++i) quotient[i] = 0;
    for (i = deg_n; i >= deg_d; --i) {
        seq_t coeff = rem[i];           /* leading coef divides exactly */
        quotient[i - deg_d] = coeff;
        /* subtract coeff * divisor shifted by (i - deg_d) */
        size_t k;
        for (k = 0; k <= deg_d; ++k) {
            rem[(i - deg_d) + k] -= coeff * divisor[k];
        }
    }
}

/* Compute Euler's totient φ(n) for n <= 42 (used only to size the array). */
static int phi_value(int n)
{
    int m = n, result = n;
    int p;
    for (p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

/* Recursive construction of Φ_n for all n up to 42. */
static void compute_phi(int n,
                        seq_t poly_cache[43][13],
                        int deg_cache[43],
                        bool computed[43])
{
    if (computed[n]) return;
    /* Base: Φ_1(x) = x - 1 */
    if (n == 1) {
        poly_cache[1][0] = -1;
        poly_cache[1][1] = 1;
        deg_cache[1] = 1;
        computed[1] = true;
        return;
    }
    /* Compute all divisors d < n */
    int divisor[43];
    int nd = 0;
    int d;
    for (d = 1; d <= n; ++d) {
        if (n % d == 0 && d < n) {
            divisor[nd++] = d;
            compute_phi(d, poly_cache, deg_cache, computed);
        }
    }
    /* Build numerator: x^n - 1 */
    seq_t num[43];
    seq_t denom[43];
    int i;
    for (i = 0; i <= n; ++i) num[i] = 0;
    num[0] = -1;
    num[n] = 1;
    /* Start denom as 1 */
    for (i = 0; i <= 42; ++i) denom[i] = 0;
    denom[0] = 1;
    int deg_den = 0;
    /* Multiply all lower Φ_d into denom */
    for (i = 0; i < nd; ++i) {
        int d = divisor[i];
        int deg_d = deg_cache[d];
        seq_t temp[43];
        poly_mul(denom, deg_den,
                 poly_cache[d], deg_d,
                 temp);
        /* copy back */
        for (int k = 0; k <= deg_den + deg_d; ++k) denom[k] = temp[k];
        deg_den += deg_d;
    }
    /* Divide numerator by denom */
    int deg_res = n - deg_den;
    seq_t res[43];
    for (i = 0; i <= deg_res; ++i) res[i] = 0;
    poly_div(num, n, denom, deg_den, res);
    /* Store result */
    for (i = 0; i <= deg_res; ++i) {
        poly_cache[n][i] = res[i];
    }
    deg_cache[n] = deg_res;
    computed[n] = true;
}

/* Public function required by the benchmark. */
seq_t a(idx_t n)
{
    /* Static cache for Φ_42 coefficients */
    static seq_t coeffs[13];
    static bool ready = false;
    if (!ready) {
        /* Prepare arrays for all lower Φ values up to 42 */
        static seq_t poly_cache[43][13];
        static int deg_cache[43];
        static bool computed[43] = { false };
        for (int i = 0; i < 43; ++i) deg_cache[i] = 0;
        compute_phi(42, poly_cache, deg_cache, computed);
        for (int i = 0; i <= deg_cache[42]; ++i) {
            coeffs[i] = poly_cache[42][i];
        }
        ready = true;
    }
    /* Return the coefficient if within bounds, otherwise 0. */
    if (n <= 12) {
        return coeffs[(size_t)n];
    } else {
        return (seq_t)0;
    }
}
