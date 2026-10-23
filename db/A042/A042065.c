/*
 * A042065  Denominators of continued fraction convergents to sqrt(556).
 * https://oeis.org/A042065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 86,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1;
   For n≥2: a(n) = cf[(n-1) % P] * a(n-1) + a(n-2)
   where cf[0..P-1] are the continued‑fraction partial quotients of √556
   (P = 36: 1,1,2,1,1,1,3,3,2,1,5,5,15,1,1,8,1,10,1,8,1,1,15,1,1,5,5,2,3,3,1,1,1,2,1,46). */

#include <stddef.h>
#include "../utils/mul_small.h"

/* Pre‑computed continued‑fraction coefficients for √556. */
static seq_t cf_coeff[36];
static idx_t cf_size = 0;

/* Initialise cf_coeff[] once. */
static void init_cf(void)
{
    if (cf_size != 0)          /* already done */
        return;

    const idx_t D   = 556;
    const idx_t a0  = 23;      /* floor(√556) */
    idx_t m = 0, d = 1, a = a0;
    idx_t i = 0;

    do
    {
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        cf_coeff[i++] = (seq_t)a_next;       /* store as seq_t */
        if (a_next == 2 * a0)                /* last term of the period */
            break;

        m   = m_next;
        d   = d_next;
        a   = a_next;
    } while (1);

    cf_size = i;   /* should be 36 */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    init_cf();                            /* ensure cf_coeff ready */

    const idx_t period = cf_size;          /* 36 */

    /* q0 = 1, q1 = a1 = cf_coeff[0] */
    seq_t q_prev2 = (seq_t)1;              /* q_{k-2} */
    seq_t q_prev1 = cf_coeff[0];           /* q_{k-1} */

    /* iterate from k = 2 to n */
    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t ak   = cf_coeff[(k - 1) % period];   /* a_k */

        seq_t prod = mul_small(q_prev1, (unsigned int)ak);   /* a_k * q_{k-1} */
        seq_t q_curr = prod + q_prev2;                     /* q_k */

        /* slide one step */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }

    return q_prev1;                     /* q_n */
}
