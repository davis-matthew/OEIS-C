/*
 * A042502  Numerators of continued fraction convergents to sqrt(779).
 * https://oeis.org/A042502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = Numerators of continued fraction convergents to sqrt(779)
 *            a(0)=27, a(n)=a(n-1)*a_n + a(n-2) where a_n are the continued
 *            fraction coefficients of sqrt(779)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 197 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    const uint64_t N   = 779ULL;
    const uint64_t a0  = 27ULL;        /* floor(sqrt(779)) */

    /* Continued fraction state */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t ai_prev = a0;

    /* Convergent numerators */
    seq_t p_nm2 = 0;   /* p_{-2} */
    seq_t p_nm1 = 1;   /* p_{-1} */
    seq_t p_curr = 0;  /* p_n */

    for (idx_t i = 0; i <= n; ++i) {
        uint64_t ai;
        if (i == 0) {
            ai = ai_prev;   /* a_0 */
        } else {
            /* Generate next CF coefficient a_i using the classic algorithm */
            m = d * ai_prev - m;
            d = (N - m * m) / d;
            ai = (a0 + m) / d;
            ai_prev = ai;
        }

        /* p_n = a_n * p_{n-1} + p_{n-2} */
        p_curr = mul_small(p_nm1, (unsigned int)ai) + p_nm2;

        /* Shift for next iteration */
        p_nm2 = p_nm1;
        p_nm1 = p_curr;
    }

    return p_curr;
}
