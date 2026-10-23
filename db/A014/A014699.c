/*
 * A014699  Inverse of 690th cyclotomic polynomial.
 * https://oeis.org/A014699
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

/*  Sequence A014699
    a(n) = coefficient of x^n in the power‑series expansion of 1/Φ_690(x),
    where Φ_690(x) is the 690th cyclotomic polynomial.
    The sequence is periodic with period 690.  */
/*  The implementation precomputes Φ_690 and uses the linear
    recurrence a(m) = -∑_{i=1}^{φ(690)} φ_i · a(m-i).  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* constant parameter */
#define P            690
#define MAX_ARRAY    (P+1)     /* 691  */
#define NUM_DIV      16

/* helper: multiply two polynomials a(x)·b(x)  (degrees da, db)
   result is written to c[0..da+db]  */
static void poly_mul(const seq_t *a, size_t da,
                     const seq_t *b, size_t db,
                     seq_t *c, size_t *dc)
{
    size_t d = da + db;
    for (size_t i = 0; i <= d; ++i) c[i] = 0;
    for (size_t i = 0; i <= da; ++i)
        for (size_t j = 0; j <= db; ++j)
            c[i + j] += a[i] * b[j];
    *dc = d;
}

/* helper: divide num(x) by monic denominator den(x)
   (deg_den <= deg_num).  The quotient is written to res.  */
static void poly_div_exact(const seq_t *num, size_t dnum,
                           const seq_t *den, size_t dden,
                           seq_t *res, size_t *dres)
{
    seq_t tmp[MAX_ARRAY];
    for (size_t i = 0; i <= dnum; ++i) tmp[i] = num[i];

    seq_t out[MAX_ARRAY];
    for (size_t i = 0; i <= dnum; ++i) out[i] = 0;

    for (idx_t i = dnum; i >= (idx_t)dden; --i) {
        seq_t coeff = tmp[i];
        if (coeff != 0) {
            size_t shift = (size_t)(i - dden);
            out[shift] = coeff;
            for (size_t j = 0; j <= dden; ++j)
                tmp[shift + j] -= coeff * den[j];
        }
    }
    *dres = dnum - dden;
    for (size_t i = 0; i <= *dres; ++i) res[i] = out[i];
}

/* static cache of sequence values (period 690) */
static seq_t cache[P];
static bool cache_initialized = false;

/* the list of divisors of 690 in ascending order */
static const idx_t divisors[NUM_DIV] =
{ 1, 2, 3, 5, 6, 10, 15, 23, 30, 46, 69, 115, 138, 230, 345, 690 };

/* initialise the cache once */
static void init_cache(void)
{
    if (cache_initialized) return;
    cache_initialized = true;

    /* arrays for polynomial computations */
    seq_t phi[NUM_DIV][MAX_ARRAY];
    size_t phi_deg[NUM_DIV];
    seq_t tmp[MAX_ARRAY], prod[MAX_ARRAY], num[MAX_ARRAY], res[MAX_ARRAY];

    /* compute Φ_d(x) for every divisor d of 690 */
    for (size_t k = 0; k < NUM_DIV; ++k) {
        idx_t d = divisors[k];
        if (d == 1) {                  /* Φ₁(x) = x – 1 */
            for (size_t i = 0; i < MAX_ARRAY; ++i) phi[k][i] = 0;
            phi[k][0] = -1;
            phi[k][1] = 1;
            phi_deg[k] = 1;
        } else {
            size_t dsize = (size_t)d;

            /* numerator: x^d – 1 */
            for (size_t i = 0; i <= dsize; ++i) num[i] = 0;
            for (size_t i = dsize + 1; i < MAX_ARRAY; ++i) num[i] = 0;
            num[0]   = -1;
            num[dsize] = 1;
            size_t num_deg = dsize;

            /* product of all Φ_m(x) where m|d, m<d */
            for (size_t i = 0; i < MAX_ARRAY; ++i) prod[i] = 0;
            prod[0] = 1;
            size_t prod_deg = 0;

            for (size_t j = 0; j < k; ++j) {
                if (d % divisors[j] == 0) {
                    size_t new_deg;
                    poly_mul(prod, prod_deg, phi[j], phi_deg[j], tmp, &new_deg);
                    /* copy back into prod */
                    for (size_t t = 0; t <= new_deg; ++t) prod[t] = tmp[t];
                    for (size_t t = new_deg + 1; t < MAX_ARRAY; ++t) prod[t] = 0;
                    prod_deg = new_deg;
                }
            }

            /* divide numerator by the product */
            size_t res_deg;
            poly_div_exact(num, num_deg, prod, prod_deg, res, &res_deg);

            /* store Φ_d(x) */
            phi_deg[k] = res_deg;
            for (size_t i = 0; i <= res_deg; ++i) phi[k][i] = res[i];
            for (size_t i = res_deg + 1; i < MAX_ARRAY; ++i) phi[k][i] = 0;
        }
    }

    /* find the index of Φ_690 */
    size_t idxN = 0;
    for (size_t i = 0; i < NUM_DIV; ++i)
        if (divisors[i] == P) { idxN = i; break; }

    /* recurrence coefficients φ_i = Φ_690[i] for i≥1 */
    size_t phi_d = phi_deg[idxN];

    /* compute the periodic cache */
    cache[0] = 1;
    for (size_t m = 1; m < P; ++m) {
        seq_t sum = 0;
        size_t lim = (phi_d < m) ? phi_d : m;
        for (size_t i = 1; i <= lim; ++i)
            sum += phi[idxN][i] * cache[m - i];
        cache[m] = -sum;
    }
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    init_cache();                        /* guarantees cache ready */
    size_t index = (size_t)(n % P);
    return cache[index];
}
