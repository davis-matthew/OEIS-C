/*
 * A014114  Inverse of 105th cyclotomic polynomial.
 * https://oeis.org/A014114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   compute_phi105, mul_by_xd_minus1, poly_divide_monic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the coefficient of x^n in the power‑series expansion of the reciprocal
   of the 105th cyclotomic polynomial Φ105(x).  It satisfies
   a(0) = 1 and for n ≥ 1
        a(n) = –∑_{k=1}^{48} φ_k · a(n–k),
   where φ_k are the coefficients of Φ105(x) (φ_0 = 1).  The values repeat
   with period 105.  This implementation builds φ_k once and then
   evaluates a(n) by this periodic recurrence. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>   /* for memcpy, memset */
#include "../utils/compute_phi105.h"
#include "../utils/mul_by_xd_minus1.h"
#include "../utils/poly_divide_monic.h"

seq_t a(idx_t n)
{
    /* Cache one full period (length 105).  The period and the polynomial
       coefficients are computed lazily on first call. */
    static seq_t period[105];
    static bool ready = false;

    if (!ready) {
        /* 1. compute coefficients φ_k of Φ105(x) */
        static seq_t phi[49];
        compute_phi105(phi);

        /* 2. compute the period by the recurrence a(n) = –∑ φ_k * a(n-k) */
        period[0] = 1;  /* a(0) = 1 */
        for (idx_t i = 1; i < 105; ++i) {
            seq_t sum = 0;
            idx_t kmax = (i < 48) ? i : 48;
            for (idx_t k = 1; k <= kmax; ++k)
                sum -= phi[k] * period[i - k];
            period[i] = sum;
        }
        ready = true;
    }

    return period[n % 105];
}
