/*
 * A015045  Inverse of 1036th cyclotomic polynomial.
 * https://oeis.org/A015045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the series expansion of 1/Φ_1036(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   poly_divide
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Φ_1036(x) is the 1036th cyclotomic polynomial; the resulting sequence is
   periodic with period 1036.  The recurrence a(n) = -∑_{k=1}^{deg} c_k
   a(n-k) with c_0 = 1 and c_k the coefficients of Φ_1036 is used. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/poly_divide.h"

#define PERIOD 1036

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t a_arr[PERIOD];
    static seq_t coeff[1037];    /* coefficients of Φ_1036, size MAX_N+1 */
    static int poly_deg[64];     /* degree of each Φ_d */

    if (!initialized) {
        /* 1. Compute divisors of PERIOD */
        int divs[64];
        int dcnt = 0;
        for (idx_t d = 1; d <= PERIOD; ++d) {
            if (PERIOD % (uint64_t)d == 0) {
                divs[dcnt++] = (int)d;
            }
        }
        /* mapping from divisor value to its index in divs[ ] */
        int idx_map[PERIOD + 1];
        for (int i = 0; i <= PERIOD; ++i) idx_map[i] = -1;
        for (int i = 0; i < dcnt; ++i) idx_map[divs[i]] = i;

        /* 2. Compute all Φ_d for d in ascending order */
        seq_t polys[64][1037];          /* store coefficients up to degree 1036 */
        for (int i = 0; i < dcnt; ++i) {
            int d = divs[i];
            int deg = d;
            seq_t tmp[1037];
            for (int j = 0; j <= d; ++j) tmp[j] = 0;
            tmp[0] = -1;     /* x^d - 1, constant term -1 */
            tmp[d] = 1;      /* leading term 1 */
            int cur_deg = d;
            for (int j = 0; j < i; ++j) {
                int k = divs[j];
                if (d % k == 0) {
                    seq_t q[1037];
                    int qdeg;
                    poly_divide(tmp, cur_deg, polys[j], poly_deg[j], q, &qdeg);
                    for (int t = 0; t <= qdeg; ++t) tmp[t] = q[t];
                    for (int t = qdeg + 1; t <= d; ++t) tmp[t] = 0;
                    cur_deg = qdeg;
                }
            }
            poly_deg[i] = cur_deg;
            for (int t = 0; t <= cur_deg; ++t) polys[i][t] = tmp[t];
        }

        /* 3. Extract Φ_1036 coefficients */
        int idx1036 = idx_map[PERIOD];
        int deg1036 = poly_deg[idx1036];
        for (int i = 0; i <= deg1036; ++i) coeff[i] = polys[idx1036][i];
        /* 4. Compute one period of sequence a(n) using recurrence */
        a_arr[0] = 1;        /* a(0) = 1 (since Φ has constant term 1) */
        for (int nn = 1; nn < PERIOD; ++nn) {
            seq_t sum = 0;
            for (int k = 1; k <= deg1036; ++k) {
                if (nn >= k) sum += coeff[k] * a_arr[nn - k];
            }
            a_arr[nn] = -sum;
        }
        initialized = true;
    }
    return a_arr[n % PERIOD];
}
