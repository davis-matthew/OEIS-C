/*
 * A015692  Inverse of 1683rd cyclotomic polynomial.
 * https://oeis.org/A015692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in the formal power series 1/Φ_1683(x),
 * where Φ_1683(x) is the 1683rd cyclotomic polynomial.  The sequence is
 * periodic with period 1683; a(0)=1.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------------- Helper polynomial routines ---------------- */

static void poly_mult(const seq_t *a, size_t da,
                      const seq_t *b, size_t db,
                      seq_t *out)
{
    size_t i, j;
    for (i = 0; i <= da + db; ++i) out[i] = 0;
    for (i = 0; i <= da; ++i) {
        for (j = 0; j <= db; ++j) {
            out[i + j] += a[i] * b[j];
        }
    }
}

/* Long division of two monic polynomials.
 * num: numerator of degree nd. den: monic denominator of degree dd.
 * Result quotient of degree nd-dd is stored in out (size nd-dd+1). */
static void poly_div(const seq_t *num, size_t nd,
                     const seq_t *den, size_t dd,
                     seq_t *out)
{
    size_t qdeg = nd - dd;
    seq_t *tmp = (seq_t *)calloc(nd + 1, sizeof(seq_t));
    for (size_t i = 0; i <= nd; ++i) tmp[i] = num[i];

    for (size_t i = 0; i <= qdeg; ++i) out[i] = 0;

    /* Process degrees from high to low. Use signed type for counter. */
    long long i;
    for (i = (long long)nd; i >= (long long)dd; --i) {
        seq_t coeff = tmp[i];
        size_t qidx = (size_t)(i - dd);
        out[qidx] = coeff;          /* den[dd] == 1 */

        /* subtract coeff * den from tmp */
        size_t j;
        for (j = 0; j <= dd; ++j) {
            tmp[i - dd + j] -= coeff * den[j];
        }
    }

    free(tmp);
}

/* ----------------- Main sequence implementation ----------------- */

#define PERIOD (idx_t)1683ULL
static seq_t period[PERIOD];
static bool period_initialized = false;

static void init_period(void)
{
    /* All divisors of 1683 in ascending order. */
    const idx_t divisors[12] = {
        1, 3, 9, 11, 17,
        33, 51, 99, 153, 187,
        561, 1683
    };
    const size_t ndiv = 12;

    /* Temporary storage for Φ_d polynomials. */
    struct {
        size_t deg;
        seq_t *coef;
    } polys[ndiv];
    for (size_t i = 0; i < ndiv; ++i) {
        polys[i].deg = 0;
        polys[i].coef = NULL;
    }

    /* Compute Φ_d for each divisor d. */
    size_t i;
    for (i = 0; i < ndiv; ++i) {
        idx_t d = divisors[i];

        if (d == 1) {
            /* Φ_1(x) = x - 1 */
            polys[i].deg = 1;
            polys[i].coef = (seq_t *)malloc(2 * sizeof(seq_t));
            polys[i].coef[0] = (seq_t)-1;
            polys[i].coef[1] = (seq_t)1;
            continue;
        }

        /* Build numerator: x^d - 1 */
        size_t num_deg = (size_t)d;
        seq_t *num = (seq_t *)calloc(num_deg + 1, sizeof(seq_t));
        num[0] = (seq_t)-1;
        num[num_deg] = (seq_t)1;

        /* Build denominator: product_{e|d, e<d} Φ_e */
        seq_t *den = (seq_t *)malloc(sizeof(seq_t));
        den[0] = (seq_t)1;
        size_t den_deg = 0;

        for (size_t eidx = 0; eidx < i; ++eidx) {
            idx_t e = divisors[eidx];
            if (d % e == 0) {
                size_t new_deg = den_deg + polys[eidx].deg;
                seq_t *new_den = (seq_t *)calloc(new_deg + 1, sizeof(seq_t));
                poly_mult(den, den_deg, polys[eidx].coef,
                          polys[eidx].deg, new_den);
                free(den);
                den = new_den;
                den_deg = new_deg;
            }
        }

        /* Divide numerator by denominator to get Φ_d. */
        size_t quo_deg = num_deg - den_deg;
        seq_t *quo = (seq_t *)calloc(quo_deg + 1, sizeof(seq_t));
        poly_div(num, num_deg, den, den_deg, quo);

        free(num);
        free(den);

        polys[i].deg = quo_deg;
        polys[i].coef = quo;
    }

    /* Extract coefficients of Φ_1683 (last divisor). */
    size_t max_deg = polys[ndiv - 1].deg;   /* should be 960 */
    seq_t p[961];
    for (size_t j = 0; j <= max_deg; ++j) {
        p[j] = polys[ndiv - 1].coef[j];
    }

    /* Compute one full period using the recurrence.
       Φ_1683(x) = 1 + c1 x + c2 x^2 + ... + c_{max_deg} x^{max_deg}. */
    period[0] = (seq_t)1;    /* a(0) */
    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t max_i = (n <= (idx_t)max_deg) ? (size_t)n : max_deg;
        for (size_t j = 1; j <= max_i; ++j) {
            sum += p[j] * period[n - j];
        }
        period[n] = -sum;   /* because p[0] == 1 */
    }

    /* Free all temporary polynomials. */
    for (size_t i = 0; i < ndiv; ++i) {
        free(polys[i].coef);
    }

    period_initialized = true;
}

seq_t a(idx_t n)
{
    if (!period_initialized) {
        init_period();
    }
    idx_t idx = n % PERIOD;
    return period[idx];
}
