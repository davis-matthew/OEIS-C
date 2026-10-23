/*
 * A042342  Numerators of continued fraction convergents to sqrt(698).
 * https://oeis.org/A042342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued‑fraction convergents to sqrt(698)
   a(0) = 26, a(1) = 53.
   For n ≥ 2:  a(n) = c_n · a(n‑1) + a(n‑2),
   where c_n is the nth partial quotient of sqrt(698).  */

#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* sqrt(698) parameters */
    const uint64_t D   = 698ULL;   /* the radicand */
    const unsigned A0 = 26U;      /* floor(sqrt(D)) */

    if (n == 0) {
        return (seq_t)A0;
    }

    /* variables for the continued‑fraction expansion */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_partial = A0;   /* a_0 */

    /* convergent numerators:  p_{-1}=1, p_0=A0 */
    seq_t p_nm2 = (seq_t)1;          /* p_{-1} */
    seq_t p_nm1 = (seq_t)A0;        /* p_0 */

    seq_t p_n;
    for (idx_t i = 1; i <= n; ++i) {
        /* compute next partial quotient a_i */
        m = d * a_partial - m;
        d = (D - m * m) / d;
        a_partial = (A0 + m) / d;   /* floor division */

        /* recurrence:  p_i = a_i * p_{i-1} + p_{i-2} */
        seq_t tmp = mul_small(p_nm1, (unsigned int)a_partial);
        p_n = tmp + p_nm2;

        /* shift for next step */
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }

    return p_nm1;
}
