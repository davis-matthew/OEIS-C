/*
 * A016400  399th cyclotomic polynomial.
 * https://oeis.org/A016400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 399th cyclotomic polynomial
 *            Φ₃₉₉(x)
 * Range:     i8 n <= 216, i16 n <= 216, i32 n <= 216, i64 n <= 216,
 *            i128 n <= 216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *phi_399 = NULL;      /* coefficients of Φ₃₉₉, lowest term first (index 0) */
static size_t deg_phi_399 = 0;     /* degree of Φ₃₉₉ */

/* multiply two polynomials a (degree da) and b (degree db), result in out (degree da+db + 1)   */
static void poly_mul(const seq_t *a, size_t da,
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

/* compute all cyclotomic polynomials for numbers dividing 399,
   store Φ₃₉₉ in the static variables phi_399 and deg_phi_399   */
static void init_phi_399(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* list of divisors of 399, sorted increasingly */
    const idx_t divisors[] = {1, 3, 7, 19, 21, 57, 133, 399};
    const size_t dcount = sizeof(divisors) / sizeof(divisors[0]);

    /* mapping from divisor value to its index in polys array */
    idx_t map[400];
    for (size_t i = 0; i < 400; ++i) map[i] = -1;
    for (size_t i = 0; i < dcount; ++i) map[divisors[i]] = (idx_t)i;

    /* arrays to hold polynomials and their degrees */
    seq_t **polys = (seq_t **)malloc(dcount * sizeof(seq_t *));
    size_t *deg  = (size_t *)malloc(dcount * sizeof(size_t));

    /* temporary buffers (max degree needed is 399) */
    seq_t Q[400], Qnew[400];
    seq_t P[400], Res[400];
    seq_t Temp[400];
    size_t i, j, k;
    (void)Temp; /* silence unused warning if not needed */

    for (size_t idx = 0; idx < dcount; ++idx) {
        idx_t n = divisors[idx];
        if (n == 1) {   /* Φ₁(x) = x - 1 */
            polys[idx] = (seq_t *)malloc(2 * sizeof(seq_t));
            polys[idx][0] = (seq_t)-1;
            polys[idx][1] = (seq_t)1;
            deg[idx] = 1;
            continue;
        }

        /* compute product of Φ_d(x) for proper divisors d < n */
        size_t Qdeg = 0;
        Q[0] = (seq_t)1;   /* multiplicative identity */

        for (size_t jdx = 0; jdx < idx; ++jdx) {
            idx_t d = divisors[jdx];
            if (n % d != 0) continue;
            seq_t *dcoef = polys[jdx];
            size_t ddeg  = deg[jdx];
            /* Qnew = Q * Φ_d */
            poly_mul(Q, Qdeg, dcoef, ddeg, Qnew);
            /* copy back to Q */
            Qdeg = Qdeg + ddeg;
            for (size_t t = 0; t <= Qdeg; ++t) Q[t] = Qnew[t];
        }

        /* Build P(x) = x^n - 1 */
        for (i = 0; i <= (size_t)n; ++i) P[i] = 0;
        P[0] = (seq_t)-1;
        P[(size_t)n] = (seq_t)1;

        /* Long division: P / Q  (Q leading coefficient is 1) */
        size_t Rdeg = (size_t)n - Qdeg;
        for (i = 0; i <= Rdeg; ++i) Res[i] = 0;

        for (k = (size_t)n; k >= Qdeg; --k) {
            seq_t coeff = P[k];                /* coefficient of x^(k-Qdeg) */
            Res[k - Qdeg] = coeff;
            /* subtract coeff * Q from P at positions k-Qdeg .. k-Qdeg+Qdeg */
            for (i = 0; i <= Qdeg; ++i) {
                P[(k - Qdeg) + i] -= coeff * Q[i];
            }
        }

        /* store result */
        polys[idx] = (seq_t *)malloc((Rdeg + 1) * sizeof(seq_t));
        for (i = 0; i <= Rdeg; ++i) polys[idx][i] = Res[i];
        deg[idx] = Rdeg;
    }

    /* store pointer to Φ₃₉₉ */
    const size_t idx399 = map[399];
    phi_399     = polys[idx399];
    deg_phi_399 = deg[idx399];

    /* free supporting arrays (polys and deg arrays themselves, but not the polynomials) */
    free(polys);
    free(deg);
}

/* sequence function */
seq_t a(idx_t n)
{
    init_phi_399();   /* compute once */

    if (n <= deg_phi_399) {
        return phi_399[(size_t)n];
    } else {
        return (seq_t)0;
    }
}
