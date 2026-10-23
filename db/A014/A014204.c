/*
 * A014204  Inverse of 195th cyclotomic polynomial.
 * https://oeis.org/A014204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the formal power series
 *            expansion of 1 / Φ_195(x), where Φ_195(x) is the 195‑th
 *            cyclotomic polynomial. The resulting sequence is periodic of
 *            period 195, so a(n) = a(n mod 195). The implementation computes
 *            the coefficients of Φ_195(x) once, inverts the series to o...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*  Local helper: Möbius function for small arguments (≤195).  */
static int mu_of(idx_t m)
{
    int sgn = 1;
    idx_t t = m;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            t /= p;
            if (t % p == 0) return 0;
            sgn = -sgn;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) sgn = -sgn;
    return sgn;
}

/*  Storage for the coefficients of Φ_195(x).  */
static seq_t *phi_coeff_195 = NULL;   /* length = phi_deg_195 + 1 */
static idx_t phi_deg_195   = 0;
static bool  phi_computed  = false;

/*  Compute Φ_195(x) = Π_{d|195} (x^d - 1)^{μ(195/d)}  */
static void compute_phi_195(void)
{
    if (phi_computed) return;
    const idx_t n = 195;

    /*  Start with numerator and denominator polynomials = 1.  */
    seq_t *num = (seq_t*)malloc(sizeof(seq_t));
    num[0] = 1;
    size_t deg_num = 0;

    seq_t *den = (seq_t*)malloc(sizeof(seq_t));
    den[0] = 1;
    size_t deg_den = 0;

    /*  Multiply over all divisors d of n.  */
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        int mu = mu_of(n / d);
        if (mu == 1) {   /* multiply numerator by (x^d - 1) */
            size_t new_deg = deg_num + d;
            seq_t *new_num = (seq_t*)calloc(new_deg + 1, sizeof(seq_t));
            for (size_t i = 0; i <= deg_num; ++i) {
                new_num[i]     -= num[i];
                new_num[i + d] += num[i];
            }
            free(num);
            num = new_num;
            deg_num = new_deg;
        } else if (mu == -1) {   /* multiply denominator by (x^d - 1) */
            size_t new_deg = deg_den + d;
            seq_t *new_den = (seq_t*)calloc(new_deg + 1, sizeof(seq_t));
            for (size_t i = 0; i <= deg_den; ++i) {
                new_den[i]     -= den[i];
                new_den[i + d] += den[i];
            }
            free(den);
            den = new_den;
            deg_den = new_deg;
        }
        /* mu==0: skip */
    }

    /*  Long division: num / den  (den is monic).  */
    size_t deg_q = (deg_num >= deg_den) ? (deg_num - deg_den) : 0;
    seq_t *q = (seq_t*)calloc(deg_q + 1, sizeof(seq_t));

    for (size_t i = deg_num; ; --i) {
        seq_t coeff = num[i];
        size_t qidx = i - deg_den;
        q[qidx] = coeff;
        if (coeff != 0) {
            for (size_t j = 0; j <= deg_den; ++j) {
                num[qidx + j] -= coeff * den[j];
            }
        }
        if (i == deg_den) break;
    }

    /*  Store results and free temporaries.  */
    free(den);
    free(num);
    phi_coeff_195 = q;
    phi_deg_195   = deg_q;   /* should be φ(195)=96 */
    phi_computed  = true;
}

/*  Periodic sequence a(n).  */
static seq_t period[195];
static bool period_computed = false;

static void compute_period(void)
{
    if (period_computed) return;
    if (!phi_computed) compute_phi_195();

    period[0] = 1;   /* a(0) = 1 */
    for (size_t k = 1; k < 195; ++k) {
        seq_t sum = 0;
        size_t maxi = (k <= phi_deg_195) ? k : phi_deg_195;
        for (size_t i = 1; i <= maxi; ++i) {
            sum += phi_coeff_195[i] * period[k - i];
        }
        period[k] = -sum;
    }
    period_computed = true;
}

/*  Public function required by the problem statement.  */
seq_t a(idx_t n)
{
    compute_period();
    return period[n % 195];
}
