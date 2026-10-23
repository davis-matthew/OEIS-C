/*
 * A015686  Inverse of 1677th cyclotomic polynomial.
 * https://oeis.org/A015686
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

/* a(n) = coefficient of x^n in the formal power series expansion of 1/Φ_1677(x),
   where Φ_1677(x) is the 1677th cyclotomic polynomial.
   The sequence is periodic with period 1677. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- polynomial helpers ------------------------------------ */

static void poly_mul(const seq_t *a, size_t da,
                     const seq_t *b, size_t db,
                     seq_t *out)
{
    size_t i, j;
    size_t outdeg = da + db;
    for (i = 0; i <= outdeg; ++i) out[i] = 0;
    for (i = 0; i <= da; ++i)
        for (j = 0; j <= db; ++j)
            out[i + j] += a[i] * b[j];
}

/* exact division; divisor is monic.   quotient = num / den   */
static void poly_div(const seq_t *num, size_t ndeg,
                     const seq_t *den, size_t ddeg,
                     seq_t *quot)
{
    size_t qdeg = ndeg - ddeg;
    seq_t *rem = (seq_t*)malloc((ndeg + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= ndeg; ++i) rem[i] = num[i];

    for (long i = (long)ndeg; i >= (long)ddeg; --i) {
        seq_t coeff = rem[i];
        size_t qpos = (size_t)(i - ddeg);
        quot[qpos] = coeff;
        for (size_t j = 0; j <= ddeg; ++j)
            rem[qpos + j] -= coeff * den[j];
    }
    free(rem);
}

/* ---------- sequence implementation -------------------------------- */

seq_t a(idx_t n)
{
    enum { PERIOD = 1677U };
    static bool initialized = false;
    static seq_t a_vals[PERIOD];
    static seq_t *phi = NULL;          /* coefficients of Φ_1677 */
    static size_t phi_deg = 0;

    if (!initialized) {
        /* Divisors of 1677 in ascending order */
        const int divs[] = {1, 3, 13, 39, 43, 129, 559, 1677};
        const int nd = sizeof(divs) / sizeof(divs[0]);

        seq_t *polys[nd];      /* stores Φ_d for divisor d */
        size_t degs[nd];

        for (int idx = 0; idx < nd; ++idx) {
            int d = divs[idx];
            if (d == 1) {            /* Φ₁(x) = x – 1 */
                polys[idx] = (seq_t*)malloc(2 * sizeof(seq_t));
                polys[idx][0] = -1;
                polys[idx][1] = 1;
                degs[idx] = 1;
                continue;
            }

            /* numerator: x^d – 1 */
            seq_t *num = (seq_t*)calloc(d + 1, sizeof(seq_t));
            num[0] = -1;
            num[d] = 1;
            size_t num_deg = d;

            /* product of earlier Φ_e where e|d */
            seq_t *prod = (seq_t*)malloc(1 * sizeof(seq_t));
            prod[0] = 1;
            size_t prod_deg = 0;

            for (int j = 0; j < idx; ++j) {
                int e = divs[j];
                if (d % e == 0) {
                    size_t new_deg = prod_deg + degs[j];
                    seq_t *tmp = (seq_t*)calloc(new_deg + 1, sizeof(seq_t));
                    poly_mul(prod, prod_deg, polys[j], degs[j], tmp);
                    free(prod);
                    prod = tmp;
                    prod_deg = new_deg;
                }
            }

            /* divide numerator by product to get Φ_d */
            size_t phi_len = num_deg - prod_deg + 1;
            seq_t *phi_d = (seq_t*)malloc(phi_len * sizeof(seq_t));
            poly_div(num, num_deg, prod, prod_deg, phi_d);

            /* store */
            polys[idx] = phi_d;
            degs[idx] = num_deg - prod_deg;

            free(num);
            free(prod);
        }

        /* The last entry is Φ_1677 */
        phi = polys[nd - 1];
        phi_deg = degs[nd - 1];

        /* compute the sequence values a(0)…a(PERIOD–1) */
        a_vals[0] = 1;
        for (size_t k = 1; k < PERIOD; ++k) {
            seq_t sum = 0;
            size_t max_i = (k <= phi_deg) ? k : phi_deg;
            for (size_t i = 1; i <= max_i; ++i)
                sum += phi[i] * a_vals[k - i];
            a_vals[k] = -sum;
        }

        /* free all Φ_d except the last one (which is no longer needed) */
        for (int idx = 0; idx < nd - 1; ++idx)
            free(polys[idx]);

        /* do not keep phi, all coefficients used already */
        free(phi);
        initialized = true;
    }

    return a_vals[(size_t)(n % PERIOD)];
}
