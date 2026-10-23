/*
 * A042585  Denominators of continued fraction convergents to sqrt(821).
 * https://oeis.org/A042585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = the denominator of the n-th convergent of sqrt(821)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 47, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* continued fraction for sqrt(821) */
    const uint64_t S = 821;
    const uint64_t a0 = 28;  /* floor(sqrt(821)) */

    /* Binet-like recurrence for convergents */
    seq_t p_nm2 = 0, p_nm1 = 1;  /* p_-2, p_-1 */
    seq_t q_nm2 = 1, q_nm1 = 0;  /* q_-2, q_-1 */

    uint64_t m = 0, d = 1, a = a0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai = (seq_t)a;
        seq_t q = ai * q_nm1 + q_nm2;
        if (i == n) return q;

        /* advance p, q */
        seq_t p = ai * p_nm1 + p_nm2;
        p_nm2 = p_nm1; p_nm1 = p;
        q_nm2 = q_nm1; q_nm1 = q;

        /* next term of continued fraction */
        m = d * a - m;
        d = (S - m * m) / d;
        a = (a0 + m) / d;
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
