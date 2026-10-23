/*
 * A042348  Numerators of continued fraction convergents to sqrt(701).
 * https://oeis.org/A042348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = numerator of the nth convergent to sqrt(701) */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Constant data for sqrt(701) */
    const uint64_t D     = 701;
    const uint64_t a0v   = 26;          /* floor(sqrt(701)) */

    /* --- compute one period of the continued‑fraction expansion of sqrt(701) --- */
    uint64_t period[200];
    idx_t    L = 0;
    uint64_t m = 0, d = 1, a = a0v;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0v + m) / d;
        period[L++] = a;
        if (a == 2 * a0v)  /* period finished */
            break;
    }

    /* --- compute the convergents --- */
    seq_t p_nm2 = 0;   /* p_{-2} */
    seq_t p_nm1 = 1;   /* p_{-1} */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai = (i == 0) ? (seq_t)a0v
                            : (seq_t)period[(i - 1) % L];
        seq_t p_curr = ai * p_nm1 + p_nm2;   /* p_i = a_i p_{i-1} + p_{i-2} */
        p_nm2 = p_nm1;
        p_nm1 = p_curr;
    }

    return p_nm1;      /* a(n) = numerator of nth convergent */
}
