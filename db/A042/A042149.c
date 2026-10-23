/*
 * A042149  Denominators of continued fraction convergents to sqrt(599).
 * https://oeis.org/A042149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction denominator recurrence for sqrt(599) */
/* q_n = a_n * q_{n-1} + q_{n-2};  q_{-2}=1, q_{-1}=0   */
/* a_0 = floor(sqrt(599)) = 24   */
/* sequence a(n) = q_n,  n ≥ 0   */

#include <stdint.h>
#include <stdlib.h>

static const uint16_t A0_CONST = 24;          /* floor(sqrt(599))           */
static uint16_t *period = NULL;              /* periodic part a_1 .. a_P   */
static size_t period_len = 0;                /* length of the period       */

/* Compute the periodic part of the continued fraction for sqrt(599) */
static void init_period(void)
{
    if (period != NULL)
        return;                               /* already initialized       */

    const uint32_t D = 599;
    uint64_t m = 0, d = 1, a = A0_CONST;
    size_t cap = 4;                           /* initial capacity          */
    size_t len = 0;                           /* number of coefficients    */

    period = (uint16_t *)malloc(cap * sizeof(uint16_t));
    if (!period)
        return;                               /* allocation failure        */

    /* The continued fraction for sqrt(D) repeats when a == 2*A0_CONST   */
    do {
        if (len == cap) {                     /* grow the temporary array  */
            cap <<= 1;
            uint16_t *tmp = (uint16_t *)realloc(period, cap * sizeof(uint16_t));
            if (!tmp) {                       /* allocation failure        */
                free(period);
                period = NULL;
                return;
            }
            period = tmp;
        }

        /* Transform state */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (A0_CONST + m) / d;

        period[len++] = (uint16_t)a;          /* store a_n (n ≥ 1)       */
    } while (a != 2 * A0_CONST);

    period_len = len;                         /* final number of terms   */
}

/* Compute the n-th denominator of the convergents to sqrt(599)    */
seq_t a(idx_t n)
{
    init_period();                           /* ensure period is ready   */

    /* Base values for the recurrence:
         p_{-2} = 0, p_{-1} = 1
         q_{-2} = 1, q_{-1} = 0
       We use only q (denominators) in the caller.            */

    seq_t p_nm2 = (seq_t)0;                  /* p_{-2}                  */
    seq_t p_nm1 = (seq_t)1;                  /* p_{-1}                  */
    seq_t q_nm2 = (seq_t)1;                  /* q_{-2}                  */
    seq_t q_nm1 = (seq_t)0;                  /* q_{-1}                  */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0)
            a_i = (seq_t)A0_CONST;           /* a_0                       */
        else
            a_i = (seq_t)period[(i - 1) % period_len]; /* a_i (i≥1)   */

        seq_t p_n = a_i * p_nm1 + p_nm2;     /* compute next numerator    */
        seq_t q_n = a_i * q_nm1 + q_nm2;     /* compute next denominator  */

        if (i == n)
            return q_n;                     /* return requested value    */

        /* shift for next iteration                     */
        p_nm2 = p_nm1;   p_nm1 = p_n;
        q_nm2 = q_nm1;   q_nm1 = q_n;
    }

    return (seq_t)0;                         /* should never reach here */
}
