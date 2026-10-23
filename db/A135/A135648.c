/*
 * A135648  Expansion of 1 / (x^256*p(1/x)) where p(x)=(-969 - 1894*x^15 +
 *          x^16) * (-1 - 3*x^15 + x^16)^4*(-1 - 2*x^15 + x^16)^5 * (-1 - x^15
 *          + x^16)^6.
 * https://oeis.org/A135648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) is the coefficient of x^n in the power series
 *            1/(x^256*p(1/x)) where p(x)=(-969 - 1894*x^15 + x^16) * (-1 -
 *            3*x^15 + x^16)^4 *(-1 - 2*x^15 + x^16)^5 * (-1 - x^15 + x^16)^6.
 *            This implementation evaluates the linear recurrence
 *            a(n)=−∑_{k=1}^{min(256,n‑1)} d_k*a(n‑k) with d_k the coefficient
 *            of...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper to multiply two polynomials a[0..da] and b[0..db].
   The result is written into res[0..da+db] and the degree is returned. */
static int mul_poly(int64_t *res, const int64_t *a, size_t da,
                    const int64_t *b, size_t db)
{
    size_t deg = da + db;
    for (size_t i = 0; i <= deg; ++i) res[i] = 0;
    for (size_t i = 0; i <= da; ++i) {
        for (size_t j = 0; j <= db; ++j) {
            res[i + j] += a[i] * b[j];
        }
    }
    return (int)deg;
}

seq_t a(idx_t n)
{
    /* Static data kept between calls */
    static seq_t coeff_pos[257] = {0};
    static seq_t coeff_neg[257] = {0};

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;      /* number of cached terms (a(1)..a(cache_len)) */

    static bool initialized = false;

    /* --------------------------------------------------------------------
       Build the recurrence coefficients d_k of q(x)=x^256*p(1/x) once.
       -------------------------------------------------------------------- */
    if (!initialized) {
        /* Base polynomials of degree 16 */
        int64_t f0[17]  = {0};
        int64_t f1b[17] = {0};
        int64_t f2b[17] = {0};
        int64_t f3b[17] = {0};

        /* (-969 - 1894*x^15 + x^16) */
        f0[0]  = -969; f0[15] = -1894; f0[16] = 1;

        /* (-1 - 3*x^15 + x^16) */
        f1b[0]  = -1; f1b[15] = -3; f1b[16] = 1;

        /* (-1 - 2*x^15 + x^16) */
        f2b[0]  = -1; f2b[15] = -2; f2b[16] = 1;

        /* (-1 - x^15 + x^16) */
        f3b[0]  = -1; f3b[15] = -1; f3b[16] = 1;

        /* Powers of the repeated factors */
        int64_t f1pow[257], f2pow[257], f3pow[257], tmp[257];
        size_t d1 = 16, d2 = 16, d3 = 16;

        for (size_t i = 0; i <= 16; ++i) f1pow[i] = f1b[i];
        for (int k = 1; k < 4; ++k) {
            d1 = mul_poly(tmp, f1pow, d1, f1b, 16);
            for (size_t i = 0; i <= d1; ++i) f1pow[i] = tmp[i];
        }

        for (size_t i = 0; i <= 16; ++i) f2pow[i] = f2b[i];
        for (int k = 1; k < 5; ++k) {
            d2 = mul_poly(tmp, f2pow, d2, f2b, 16);
            for (size_t i = 0; i <= d2; ++i) f2pow[i] = tmp[i];
        }

        for (size_t i = 0; i <= 16; ++i) f3pow[i] = f3b[i];
        for (int k = 1; k < 6; ++k) {
            d3 = mul_poly(tmp, f3pow, d3, f3b, 16);
            for (size_t i = 0; i <= d3; ++i) f3pow[i] = tmp[i];
        }

        /* Multiply all factors: ((f0 * f1pow) * f2pow) * f3pow */
        int64_t prod1[257], prod2[257];
        int deg1 = mul_poly(tmp, f0, 16, f1pow, d1);          /* f0 * f1pow */
        int deg2 = mul_poly(prod1, tmp, deg1, f2pow, d2);     /* * f2pow */
        mul_poly(prod2, prod1, deg2, f3pow, d3);              /* * f3pow (degree 256) */

        /* Reverse the polynomial to get q(x) = x^256 * p(1/x) */
        int64_t q[257];
        for (size_t i = 0; i <= 256; ++i) q[i] = prod2[256 - i];

        /* Store the recurrence coefficients */
        for (size_t i = 1; i <= 256; ++i) {
            if (q[i] < 0)
                coeff_neg[i] = (seq_t)(-q[i]);
            else
                coeff_pos[i] = (seq_t)q[i];
        }

        /* Allocate initial cache for first 257 terms (a(1)..a(257)) */
        cache = (seq_t *)malloc(257 * sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = 257;
        cache_len = 0;

        /* a(1) is 1 */
        cache[0] = 1;
        cache_len = 1;

        /* Compute a(2)..a(257) using the recurrence */
        for (size_t i = 1; i <= 256; ++i) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            for (size_t j = 1; j <= i; ++j) {
                seq_t val = cache[i - j];
                if (coeff_pos[j])
                    pos_sum += coeff_pos[j] * val;
                else if (coeff_neg[j])
                    neg_sum += coeff_neg[j] * val;
            }
            cache[i] = neg_sum - pos_sum;
        }
        cache_len = 257;

        initialized = true;
    }

    /* --------------------------------------------------------------------
       Extend cache if needed to cover the requested index n.
       -------------------------------------------------------------------- */
    if ((size_t)n > cache_len) {
        size_t required = (size_t)n;
        if (required > cache_cap) {
            size_t new_cap = required * 2;
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_ptr) exit(1);
            cache = new_ptr;
            cache_cap = new_cap;
        }
        for (size_t k = cache_len; k < required; ++k) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            size_t limit = k - 1 < 256 ? k - 1 : 256;
            for (size_t j = 1; j <= limit; ++j) {
                seq_t val = cache[k - j];
                if (coeff_pos[j])
                    pos_sum += coeff_pos[j] * val;
                else if (coeff_neg[j])
                    neg_sum += coeff_neg[j] * val;
            }
            cache[k] = neg_sum - pos_sum;
        }
        cache_len = required;
    }

    return cache[n - 1];
}
