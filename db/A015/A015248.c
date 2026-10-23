/*
 * A015248  Inverse of 1239th cyclotomic polynomial.
 * https://oeis.org/A015248
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
 * Sequence: A015248 – inverse of the 1239th cyclotomic polynomial
 *
 * The terms are the coefficients of the formal power series
 *     1 / Φ_1239(x),
 * where Φ_1239(x) is the 1239th cyclotomic polynomial.
 * The sequence is periodic with period 1239.
 *
 * The coefficients of Φ_1239(x) are obtained by the standard
 * recursive definition
 *
 *     Φ_d(x) = (x^d - 1) / Π_{e∣d, e<d} Φ_e(x) ,
 *
 * for all divisors d of 1239.  Then the inverse series is generated
 * by the ordinary generating‑function recurrence
 *
 *     a(0) = 1
 *     a(n) = - Σ_{k=1}^{min(n,φ)} φ_k · a(n-k)     (n ≥ 1)
 *
 * where φ_k are the coefficients of Φ_1239(x) (φ_0 = 1).
 */

/*----------  Standard headers  ----------*/
#include <stdint.h>
#include <stddef.h>

/*----------  Configuration  ----------*/
#define PERIOD      1239            /* recurrence period      */
#define MAX_DEG     1240            /* room for polynomials   */
#define NUM_DIVS    8               /* number of divisors of 1239 */

/*----------  Divisors of 1239  ----------*/
static const idx_t divisors[NUM_DIVS] = {1, 3, 7, 21, 59, 177, 413, 1239};

/*----------  Storage for Φ_d polynomials  ----------*/
static seq_t poly[NUM_DIVS][MAX_DEG];
static size_t deg[NUM_DIVS];

/*----------  Auxiliary polynomial routines  ----------*/
static void poly_mul(const seq_t a[], size_t da, const seq_t b[], size_t db,
                     seq_t res[], size_t *rdeg)
{
    size_t r = da + db;
    for (size_t i = 0; i <= r; ++i) res[i] = (seq_t)0;
    for (size_t i = 0; i <= da; ++i)
        for (size_t j = 0; j <= db; ++j)
            res[i + j] += a[i] * b[j];
    *rdeg = r;
}

static void poly_div(const seq_t N[], size_t dn,
                     const seq_t D[], size_t dd,
                     seq_t Q[], size_t *qdeg)
{
    /* D[dd] is assumed to be 1 (monic divisor) */
    seq_t R[MAX_DEG];
    for (size_t i = 0; i <= dn; ++i) R[i] = N[i];
    for (size_t i = 0; i <= dn - dd; ++i) Q[i] = (seq_t)0;

    for (idx_t i = dn; i >= dd; --i) {
        seq_t coeff = R[i];          /* coefficient of x^i in numerator */
        if (coeff != (seq_t)0) {
            Q[i - dd] = coeff;       /* D[dd] == 1 */
            for (size_t j = 0; j <= dd; ++j)
                R[i - dd + j] -= coeff * D[j];
        }
        if (i == (idx_t)dd) break;   /* avoid underflow for unsigned type */
    }

    /* compute actual degree of the quotient */
    size_t q = dn - dd;
    while (q > 0 && Q[q] == (seq_t)0) --q;
    *qdeg = q;
}

/*----------  Build all Φ_d for divisors of 1239  ----------*/
static void build_phi_all(void)
{
    seq_t product[MAX_DEG];
    seq_t temp[MAX_DEG];

    for (int i = 0; i < NUM_DIVS; ++i) {
        idx_t d = divisors[i];

        /* start with (x^d - 1) */
        for (size_t k = 0; k <= MAX_DEG; ++k) product[k] = (seq_t)0;
        product[0] = - (seq_t)1;
        product[d] = (seq_t)1;
        size_t deg_prod = d;

        /* divide by Φ_e for all proper divisors e of d */
        for (int j = 0; j < i; ++j) {
            idx_t e = divisors[j];
            if (d % e == 0) {
                poly_div(product, deg_prod, poly[j], deg[j], temp, &deg_prod);
                /* copy result back to product */
                for (size_t k = 0; k <= deg_prod; ++k) product[k] = temp[k];
            }
        }

        /* store Φ_d */
        for (size_t k = 0; k <= deg_prod; ++k) poly[i][k] = product[k];
        deg[i] = deg_prod;
    }
}

/*----------  Cache for periodic values  ----------*/
static seq_t cache[PERIOD];
static int cache_ready = 0;

/*----------  Compute the cache  ----------*/
static void compute_cache(void)
{
    /* build all Φ_d polynomials first */
    build_phi_all();

    /* φ coefficients (coefficient array of Φ_1239) */
    size_t idx_1239 = NUM_DIVS - 1;                 /* last element */
    size_t phi_deg   = deg[idx_1239];
    seq_t *phi = poly[idx_1239];

    /* compute the inverse series for the first PERIOD terms */
    seq_t a_raw[PERIOD];
    a_raw[0] = (seq_t)1;
    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = (seq_t)0;
        idx_t kmax = (n < phi_deg) ? n : (idx_t)phi_deg;
        for (idx_t k = 1; k <= kmax; ++k)
            sum += phi[k] * a_raw[n - k];
        a_raw[n] = -sum;
    }

    /* copy into static cache */
    for (idx_t i = 0; i < PERIOD; ++i) cache[i] = a_raw[i];
    cache_ready = 1;
}

/*----------  Public sequence function  ----------*/
seq_t a(idx_t n)
{
    if (!cache_ready) compute_cache();
    idx_t r = n % PERIOD;
    return cache[r];
}
