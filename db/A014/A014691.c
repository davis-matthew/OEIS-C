/*
 * A014691  Inverse of 682nd cyclotomic polynomial.
 * https://oeis.org/A014691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   multiply_by_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of the 682nd cyclotomic polynomial.  
   The sequence is defined by a(0)=1 and for n≥1
   a(n) = –∑_{k=1}^{deg} a(n−k)·c_k,
   where c_k are the coefficients of Φ₆₈₂(x).  The sequence is
   periodic with period 682. */

#include <stdlib.h>
#include "../utils/multiply_by_term.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

static const idx_t PERIOD = 682;   /* period length of the sequence */

seq_t a(idx_t n)
{
    /* static cache of one period */
    static seq_t *period = NULL;
    static int initialized = 0;

    if (!initialized) {
        /* ----- compute coefficients of Φ₆₈₂(x) via Möbius product ----- */
        const idx_t divs[8] = {1, 2, 11, 31, 22, 62, 341, 682};
        const int   mu  [8] = {-1, 1, 1, 1, -1, -1, -1, 1};

        /* start with polynomial 1 */
        seq_t *num = (seq_t*)calloc(1, sizeof(seq_t));
        num[0] = 1;
        idx_t deg_num = 0;
        seq_t *den = (seq_t*)calloc(1, sizeof(seq_t));
        den[0] = 1;
        idx_t deg_den = 0;

        for (int i = 0; i < 8; ++i) {
            if (mu[i] > 0)
                multiply_by_term(&num, &deg_num, divs[i]);   /* numerator */
            else
                multiply_by_term(&den, &deg_den, divs[i]);   /* denominator */
        }

        /* ----- divide numerator by denominator to get Φ₆₈₂(x) ----- */
        idx_t deg_res = deg_num - deg_den;    /* should be 300 */

        seq_t *tmp = (seq_t*)calloc(deg_num + 1, sizeof(seq_t));
        for (idx_t i = 0; i <= deg_num; ++i) tmp[i] = num[i];

        seq_t *phi = (seq_t*)calloc(deg_res + 1, sizeof(seq_t));

        for (idx_t i = deg_num; i >= deg_den; --i) {
            seq_t coeff = tmp[i];
            phi[i - deg_den] = coeff;

            if (coeff != 0) {
                for (idx_t k = 0; k <= deg_den; ++k) {
                    seq_t prod = coeff * den[k];
                    tmp[i - deg_den + k] -= prod;
                }
            }
            if (i == deg_den) break;   /* avoid underflow of unsigned idx_t */
        }

        /* ----- compute one full period of the sequence ----- */
        period = (seq_t*)calloc(PERIOD, sizeof(seq_t));
        period[0] = 1;   /* a(0) = 1 */
        for (idx_t k = 1; k < PERIOD; ++k) {
            seq_t val = 0;
            idx_t max_i = (k < deg_res) ? k : deg_res;
            for (idx_t i = 1; i <= max_i; ++i) {
                seq_t prod = period[k - i] * phi[i];
                val -= prod;
            }
            period[k] = val;
        }

        /* ----- clean up temporary storage ----- */
        free(num);
        free(den);
        free(tmp);
        free(phi);

        initialized = 1;
    }

    return period[n % PERIOD];
}
