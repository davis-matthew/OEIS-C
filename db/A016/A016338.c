/*
 * A016338  154th cyclotomic polynomial.
 * https://oeis.org/A016338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in the 154‑th cyclotomic
 * polynomial Φ154(x).
 * Compute Φ_d(x) for every divisor d of 154 in ascending order and
 * reuse them to obtain Φ154(x) via exact integer division.
 */
#include <stdbool.h>

static seq_t poly_mul(const seq_t a[], int da,
                      const seq_t b[], int db,
                      seq_t c[], int *dc)
{
    int d = da + db;
    for (int i = 0; i <= d; ++i) c[i] = 0;
    for (int i = 0; i <= da; ++i)
        for (int j = 0; j <= db; ++j)
            c[i + j] += a[i] * b[j];
    *dc = d;
    return 0;
}

static int poly_div_exact(const seq_t div[], int nd,
                          const seq_t den[], int ndv,
                          seq_t quo[], int *nq)
{
    int degq = nd - ndv;
    *nq = degq;
    seq_t temp[155];
    for (int i = 0; i <= nd; ++i) temp[i] = div[i];
    for (int i = 0; i <= degq; ++i) quo[i] = 0;
    for (int k = nd; k >= ndv; --k) {
        seq_t coeff = temp[k];
        quo[k - ndv] = coeff;
        if (coeff != 0) {
            for (int j = 0; j <= ndv; ++j)
                temp[k - ndv + j] -= coeff * den[j];
        }
    }
    return 0;
}

seq_t a(idx_t n)
{
    /* Divisors of 154 in increasing order: 1, 2, 7, 11, 14, 22, 77, 154 */
    const idx_t divs[8] = {1, 2, 7, 11, 14, 22, 77, 154};
    static seq_t phi[8][155];
    static idx_t deg[8];
    static bool initialized = false;

    if (!initialized) {
        /* compute Φ_d for each d in divs */
        for (int i = 0; i < 8; ++i) {
            idx_t d = divs[i];
            if (d == 1) {
                /* Φ_1 = x - 1 */
                deg[i] = 1;
                phi[i][0] = -1;
                phi[i][1] = 1;
            } else {
                /* check if d is prime */
                bool is_prime = true;
                if (d == 2 || d == 7 || d == 11) is_prime = true;
                else {
                    is_prime = false;
                }
                if (is_prime) {
                    /* Φ_p(x) = 1 + x + ... + x^(p-1) */
                    deg[i] = d - 1;
                    for (int k = 0; k <= deg[i]; ++k) phi[i][k] = 1;
                } else {
                    /* composite divisor: perform successive divisions */
                    seq_t temp[155], next[155];
                    int curdeg = (int)d;
                    for (int k = 0; k <= curdeg; ++k) temp[k] = 0;
                    temp[0] = -1;
                    temp[curdeg] = 1;   /* x^d - 1 */

                    for (int j = 0; j < i; ++j) {      /* proper divisors */
                        idx_t e = divs[j];
                        if (d % e == 0) {
                            int newdeg;
                            poly_div_exact(temp, curdeg,
                                           phi[j], (int)deg[j],
                                           next, &newdeg);
                            for (int k = 0; k <= newdeg; ++k) temp[k] = next[k];
                            curdeg = newdeg;
                        }
                    }
                    deg[i] = curdeg;
                    for (int k = 0; k <= curdeg; ++k) phi[i][k] = temp[k];
                }
            }
        }
        initialized = true;
    }

    /* The index of Φ154 */
    const int idx_phi154 = 7;
    if (n > deg[idx_phi154]) return (seq_t)0;
    return phi[idx_phi154][(int)n];
}
