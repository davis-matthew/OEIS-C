/*
 * A015304  Inverse of 1295th cyclotomic polynomial.
 * https://oeis.org/A015304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Validated: standard, pass, 97 terms through n = 1954 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A015304 – Inverse of the 1295th cyclotomic polynomial.
 *
 *  The sequence is the coefficient of x^n in the formal power series
 *  1/Φ_1295(x).  The coefficients are periodic with period 1295.
 *
 *  The implementation computes Φ_1295(x) as a product of
 *  (x^m-1) raised to ±1, using the divisor/möbius representation.
 *  It then uses the recurrence
 *
 *      a(0) = 1,
 *      a(k) = -∑_{i=1}^{min(k,deg)} Φ_1295[i] * a(k-i)    for k≥1,
 *
 *  where Φ_1295[i] are the coefficients of Φ_1295(x) and
 *  deg = Φ_1295 degree = 864.
 *
 *  The sequence values are cached in a static array of length 1295
 *  (the period), so that each call runs in O(1) time after the
 *  one‑time initialization.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: divide a polynomial by (x^k-1).  The division is exact;
 * the resulting quotient replaces the old polynomial and the old
 * array is freed.  Returns the new array. */
static seq_t *
poly_div_by_xk_minus_1(seq_t *poly, size_t *len, size_t k)
{
    size_t old_len = *len;
    size_t new_len = old_len - k;
    seq_t *quot = mem_new(seq_t, new_len);

    for (long long i = (long long)old_len - 1; i >= (long long)k; --i) {
        seq_t coeff = poly[i];          /* quotient coefficient at i-k */
        quot[(size_t)(i - k)] = coeff;
        if (coeff != 0) {
            /* subtract coeff*(x^k-1) shifted to align at degree i */
            poly[(size_t)(i - k)] += coeff;   /* i-k term gets +coeff */
        }
    }

    mem_free(poly);
    *len = new_len;
    return quot;
}

/* Helper: multiply a polynomial by (x^k-1).  The result replaces the old
 * polynomial; the old array is freed.  Returns the new array. */
static seq_t *
poly_mul_by_xk_minus_1(const seq_t *poly, size_t *len, size_t k)
{
    size_t old_len = *len;
    size_t new_len = old_len + k;
    seq_t *new_poly = mem_new(seq_t, new_len);

    for (size_t i = 0; i < new_len; ++i) {
        seq_t val = 0;
        /* contribution from -poly[i] */
        if (i < old_len) val -= poly[i];
        /* contribution from +poly[i-k]  (only if i>=k) */
        if (i >= k && (i - k) < old_len) val += poly[i - k];
        new_poly[i] = val;
    }

    mem_free(poly);
    *len = new_len;
    return new_poly;
}

/* Build Φ_1295(x) and return its coefficient array (length deg+1).              */
static void
build_phi_1295(seq_t **phi, size_t *len)
{
    /* Divisors of 1295 = 5 * 7 * 37 */
    const uint32_t divs[] = {1, 5, 7, 35, 37, 185, 259, 1295};
    const int    mus[]   = { 1, -1, -1,  1, -1,  1,  1, -1 };  /* μ(d) */
    const size_t ndiv   = sizeof(divs) / sizeof(divs[0]);

    /* Separate positive and negative exponent factors (m = 1295/d) */
    size_t pos_m[4], pos_cnt = 0;
    size_t neg_m[4], neg_cnt = 0;
    for (size_t i = 0; i < ndiv; ++i) {
        size_t m = 1295u / divs[i];
        if (mus[i] == 1) pos_m[pos_cnt++] = m;
        else              neg_m[neg_cnt++] = m;
    }

    /* Start with polynomial "1" */
    seq_t *poly = mem_new(seq_t, 1);
    poly[0]    = 1;
    size_t poly_len = 1;

    /* Multiply by all positive factors (x^m-1) */
    for (size_t i = 0; i < pos_cnt; ++i) {
        poly = poly_mul_by_xk_minus_1(poly, &poly_len, pos_m[i]);
    }

    /* Divide by all negative factors (x^m-1) */
    for (size_t i = 0; i < neg_cnt; ++i) {
        poly = poly_div_by_xk_minus_1(poly, &poly_len, neg_m[i]);
    }

    /* Result is Φ_1295(x) */
    *phi = poly;
    *len = poly_len;
}

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Main entry function: sequence value a(n). */
seq_t a(idx_t n)
{
    /* Period length declared constant in OEIS */
    const idx_t PERIOD = 1295;

    /* Static cache for first PERIOD terms */
    static seq_t *period_cache = NULL;
    static bool initialized   = false;

    if (!initialized) {
        /* 1. Compute Φ_1295(x) */
        seq_t *phi = NULL;
        size_t phi_len = 0;
        build_phi_1295(&phi, &phi_len);
        /* phi[0] should be 1 and phi_len-1 = 864 */

        /* 2. Compute inverse series coefficients a[0..PERIOD-1] */
        period_cache = mem_new(seq_t, PERIOD);

        period_cache[0] = 1;                  /* a(0) = 1 */
        for (idx_t k = 1; k < PERIOD; ++k) {
            seq_t sum = 0;
            size_t limit = (phi_len - 1 < (size_t)k) ? (phi_len - 1) : (size_t)k;
            for (size_t i = 1; i <= limit; ++i) {
                sum += phi[i] * period_cache[k - (idx_t)i];
            }
            period_cache[k] = -sum;
        }

        /* 3. Free temporary polynomial */
        mem_free(phi);

        initialized = true;
    }

    idx_t idx = n % PERIOD;
    return period_cache[idx];
}
