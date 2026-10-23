/*
 * A015674  Inverse of 1665th cyclotomic polynomial.
 * https://oeis.org/A015674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the formal power series
 *            expansion of the reciprocal of the 1665th cyclotomic polynomial
 *            Φ_1665(x). The series is periodic with period 1665. The
 *            following code computes the coefficients of Φ_1665 once, uses
 *            them to generate the first 1665 values of the sequence an...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define PERIOD 1665
/* The degree of Φ_1665 is φ(1665) = 864 */
#define DEG_PHI 864

/* Helper structure for polynomials */
typedef struct {
    seq_t *c;   /* coefficients, least significant first */
    size_t deg; /* degree of polynomial */
} Poly;

/* Divide dividend by divisor (monic), returning quotient.
   It is assumed that divisor divides dividend exactly. */
static seq_t* poly_div_exact(const seq_t *dividend, size_t deg_d,
                             const seq_t *divisor, size_t deg_s)
{
    size_t deg_q = deg_d - deg_s;
    seq_t *q = (seq_t*)calloc(deg_q + 1, sizeof(seq_t));
    seq_t *f = (seq_t*)malloc((deg_d + 1) * sizeof(seq_t));
    memcpy(f, dividend, (deg_d + 1) * sizeof(seq_t));

    for (int k = (int)deg_q; k >= 0; --k) {
        seq_t coeff = f[deg_s + k];  /* leading coeff of current remainder */
        q[k] = coeff;
        if (coeff != 0) {
            for (size_t j = 0; j <= deg_s; ++j) {
                f[k + j] -= coeff * divisor[j];
            }
        }
    }
    free(f);
    return q;
}

/* Compute the coefficients (in increasing degree) of Φ_1665(x).  The returned
   array must be freed by the caller. */ 
static seq_t* compute_phi_1665(void)
{
    /* Divisors of 1665 sorted increasingly */
    const idx_t divs[] = { 1, 3, 5, 9, 15, 37, 45, 111, 185, 333, 555, 1665 };
    const size_t D = sizeof(divs) / sizeof(divs[0]);

    Poly *polys = (Poly*)malloc(D * sizeof(Poly));

    /* φ_1(x) = x - 1 */
    seq_t *c1 = (seq_t*)malloc(2 * sizeof(seq_t));
    c1[0] = (seq_t)-1;
    c1[1] = (seq_t)1;
    polys[0].c = c1;
    polys[0].deg = 1;

    /* Compute φ_d for all other divisors */
    for (size_t idx = 1; idx < D; ++idx) {
        idx_t d = divs[idx];
        /* Start with x^d - 1 */
        seq_t *tmp = (seq_t*)calloc(d + 1, sizeof(seq_t));
        tmp[0] = (seq_t)-1;
        tmp[d] = (seq_t)1;
        size_t deg_tmp = d;

        /* Divide by all φ_e where e divides d and e < d */
        for (size_t j = 0; j < idx; ++j) {
            idx_t e = divs[j];
            if (d % e != 0) continue;
            seq_t *q = poly_div_exact(tmp, deg_tmp,
                                      polys[j].c, polys[j].deg);
            free(tmp);
            tmp = q;
            deg_tmp = deg_tmp - polys[j].deg;
        }
        polys[idx].c = tmp;
        polys[idx].deg = deg_tmp;
    }

    /* The last element corresponds to φ_1665 */
    seq_t *phi = polys[D - 1].c;

    /* Free all other intermediate polynomials */
    for (size_t i = 0; i < D - 1; ++i) {
        free(polys[i].c);
    }
    free(polys);
    return phi;
}

/* Static cache for the first 1665 coefficients of the sequence */
static seq_t *cache = NULL;

/* Initialize the cache lazily */
static void init_cache(void)
{
    if (cache) return;

    cache = (seq_t*)malloc(PERIOD * sizeof(seq_t));
    if (!cache) return; /* allocation failure */

    /* Compute the cyclotomic polynomial coefficients */
    seq_t *phi = compute_phi_1665();   /* degree 864 */

    /* Generate the periodic sequence */
    cache[0] = (seq_t)1;
    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = (seq_t)0;
        idx_t maxk = (n <= DEG_PHI) ? n : DEG_PHI;
        for (idx_t k = 1; k <= maxk; ++k) {
            sum += phi[k] * cache[n - k];
        }
        cache[n] = -sum;
    }
    free(phi);
}

/* Sequence function with external linkage */
seq_t a(idx_t n)
{
    init_cache();
    return cache[n % PERIOD];
}
