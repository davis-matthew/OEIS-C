/*
 * A034344  Number of binary [ n,3 ] codes without 0 columns.
 * https://oeis.org/A034344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A034344: Number of binary [n,3] codes without 0 columns.
 * The sequence satisfies the linear recurrence obtained from the
 * generating function
 *
 *   A(x) = (-x^15 + 2*x^14 - x^13 + x^12 + x^9 - x^7 + x^4 + x^3) /
 *          ((1 - x)^2 * (1 - x^2) * (1 - x^3)^2 * (1 - x^4) * (1 - x^7))
 *
 * Let d_j (j=1..21) be the coefficients of the denominator polynomial
 * 1 - 2x + 0x^2 + 0x^3 + 2x^4 + 2x^5 - 3x^6 - x^7 - x^8 + 2x^9 + 2x^10
 *           - 2x^11 - 2x^12 + x^13 + x^14 + 3x^15 - 2x^16 - 2x^17 + 0x^18
 *           + 0x^19 + 2x^20 - x^21
 *
 * and let b_n be the coefficient of x^n in the numerator (0 except
 * n=3,4,7,9,12,13,14,15).
 *
 * Then for n ≥ 1:
 *
 *   S_plus  = Σ_{j>0, d_j>0} d_j * a(n-j)
 *   S_minus = Σ_{j>0, d_j<0} (-d_j) * a(n-j)
 *
 *   a(n) = S_minus - S_plus + b_n            (b_n = 0 for n>15)
 *
 * The values for n = 1..24 are:
 * 0, 0, 1, 3, 6, 12, 21, 34, 54, 82, 120, 174, 244, 337, 458, 613,
 * 808, 1056, 1361, 1738, 2200, 2759, 3431, 4240, ...
 */
#include <stddef.h>
#include <stdlib.h>

/* Coefficients of the denominator polynomial (degree 21) */
static const long long d_coeff_arr[22] = {
    1,   /* x^0 */
   -2,   /* x^1 */
    0,   /* x^2 */
    0,   /* x^3 */
    2,   /* x^4 */
    2,   /* x^5 */
   -3,   /* x^6 */
   -1,   /* x^7 */
   -1,   /* x^8 */
    2,   /* x^9 */
    2,   /* x^10 */
   -2,   /* x^11 */
   -2,   /* x^12 */
    1,   /* x^13 */
    1,   /* x^14 */
    3,   /* x^15 */
   -2,   /* x^16 */
   -2,   /* x^17 */
    0,   /* x^18 */
    0,   /* x^19 */
    2,   /* x^20 */
   -1    /* x^21 */
};

/* Coefficients of the numerator polynomial (degree ≤15) */
static const int  Ncoeff_arr[22] = {
    0, 0, 0,       /* 0..2   */
    1, 1, 0, 0,    /* 3..6   */
   -1, 0, 1, 0, 0, /* 7..11  */
    1, -1, 2, -1,  /* 12..15 */
    0, 0, 0, 0, 0, /* 16..20 */
    0             /* 21     */
};

static seq_t *cache = NULL;    /* array of computed a(n) values */
static size_t cache_alloc = 0;
static idx_t  cache_max   = 0; /* highest n for which cache[n] is valid */

/* Ensure that cache contains values up to n (inclusive).  The cache
   is grown only as necessary. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;

    /* grow the cache to hold indices up to n */
    size_t new_size = cache_alloc;
    if (new_size < (size_t)(n + 1))
        new_size = (size_t)(n + 1);

    if (new_size > cache_alloc) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();           /* memory failure */
        cache = tmp;
        cache_alloc = new_size;
    }

    /* compute new entries */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t S_plus  = 0;
        seq_t S_minus = 0;

        /* accumulate contributions from d_coeff_arr[1..21] */
        size_t maxj = (i <= (idx_t)21 ? i : 21);
        for (size_t j = 1; j <= maxj; ++j) {
            long long coeff = d_coeff_arr[j];
            if (coeff == 0) continue;
            seq_t prod = cache[i - j] * (seq_t)(coeff < 0 ? -coeff : coeff);
            if (coeff > 0)
                S_plus += prod;
            else
                S_minus += prod;
        }

        /* add the nominators' contribution for n ≤ 15 */
        seq_t tmp = S_minus;
        if (i <= (idx_t)15) {
            int b = Ncoeff_arr[(size_t)i];
            if (b >= 0)
                tmp += (seq_t)b;
            else
                tmp -= (seq_t)(-b);   /* b is negative */
        }

        /* finally subtract S_plus */
        if (tmp < S_plus)
            cache[i] = 0;            /* should not happen for this recurrence */
        else
            cache[i] = tmp - S_plus;
    }

    cache_max = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
