/*
 * A014723  Inverse of 714th cyclotomic polynomial.
 * https://oeis.org/A014723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in the power series expansion of
 *                1 / Φ_{714}(x)      (inverse of the 714th cyclotomic
 *                polynomial), which is a periodic sequence of period 714.
 *
 * The coefficients of Φ_{714}(x) are obtained from the Möbius
 * product ∏_{d|714} (x^d – 1)^{μ(714/d)}.  The resulting polynomial has
 * degree φ(714)=192.  The reciprocal series is computed by the usual
 * convolution recurrence
 *
 *     a(0) = 1,
 *     a(n) = –∑_{k=1..min(n,deg)} c_k * a(n−k)   for n ≥ 1,
 *
 * where c_k are the coefficients of Φ_{714}(x).  The final sequence is
 * periodic with period 714, so only the first 714 terms are stored.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PERIOD 714
#define MAX_DEG 1000   /* sufficient for intermediate numerators/denominators */

static seq_t coeff[MAX_DEG + 1];          /* coefficients of Φ_714(x)  */
static seq_t period[PERIOD];              /* first 714 terms of the series */
static bool ready = false;

/* Multiply a polynomial by (x^d – 1).  Input: poly[0..deg], output same array. */
static void multiply_by_xd_minus_one(seq_t *poly, size_t *deg, unsigned d)
{
    size_t new_deg = *deg + d;
    static seq_t tmp[MAX_DEG + 1];

    for (size_t i = 0; i <= new_deg; ++i) tmp[i] = 0;
    for (size_t i = 0; i <= *deg; ++i) tmp[i] = poly[i];

    for (size_t i = 0; i <= new_deg; ++i) {
        seq_t val = (i <= *deg) ? tmp[i] : 0;
        seq_t sub = (i >= d && (i - d) <= *deg) ? tmp[i - d] : 0;
        poly[i] = val - sub;
    }
    *deg = new_deg;
}

/* Compute the coefficients of Φ_714(x) via the Möbius product. */
static void compute_phi_714(void)
{
    seq_t num[MAX_DEG + 1] = {0};
    seq_t den[MAX_DEG + 1] = {0};
    size_t deg_num = 0, deg_den = 0;

    num[0]  = 1;
    den[0]  = 1;

    /* divisors of 714 sorted ascending and their μ(714/d) values */
    const unsigned divs[16] = {
         1,  2,  3,  6,  7, 14, 17, 21,
        34, 42, 51, 102,119,238,357,714
    };
    const int    mu[16]   = {
          1, -1, -1,  1, -1,  1, -1,  1,
          1, -1,  1, -1,  1, -1, -1,  1
    };

    for (idx_t i = 0; i < 16; ++i) {
        unsigned d = divs[i];
        int m     = mu[i];
        if (m == 1)          /* multiply by (x^d – 1) */
            multiply_by_xd_minus_one(num, &deg_num, d);
        else if (m == -1)    /* multiply denominator by (x^d – 1) */
            multiply_by_xd_minus_one(den, &deg_den, d);
    }

    /* divide num by den exactly; den is monic */
    seq_t copy[MAX_DEG + 1];
    for (size_t i = 0; i <= deg_num; ++i) copy[i] = num[i];
    for (size_t i = deg_num + 1; i <= MAX_DEG; ++i) coeff[i] = 0;

    for (int i = (int)deg_num; i >= (int)deg_den; --i) {
        seq_t q = copy[i];
        if (q != 0) {
            coeff[i - deg_den] = q;
            for (size_t j = 0; j <= deg_den; ++j)
                copy[i - deg_den + j] -= q * den[j];
        } else {
            coeff[i - deg_den] = 0;
        }
    }
    /* remainder should be zero; trim trailing zeros */
    size_t deg_res = deg_num - deg_den;   /* should be 192 for Φ_714 */
    for (size_t i = 0; i <= deg_res; ++i) {
        /* coeff already set above, nothing else needed */
    }
}

/* Initialise period array from the coefficients of Φ_714(x). */
static void init_period(void)
{
    compute_phi_714();

    period[0] = 1;
    /* degree of Φ_714(x) is 192 */
    const size_t deg = 192;

    for (size_t m = 1; m < PERIOD; ++m) {
        seq_t s = 0;
        size_t up = (m <= deg) ? m : deg;
        for (size_t k = 1; k <= up; ++k)
            s += coeff[k] * period[m - k];
        period[m] = -s;
    }
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready)
        init_period();
    return period[(size_t)(n % PERIOD)];
}
