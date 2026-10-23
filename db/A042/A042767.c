/*
 * A042767  Denominators of continued fraction convergents to sqrt(914).
 * https://oeis.org/A042767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042767: denominators of continued‑fraction convergents to √914. */
/* a(0)=1; for n≥1, let a_k be the k‑th partial quotient (after a0=30) of √914, */
/* then a(n)=a_k*a(n‑1)+a(n‑2).  The period of the partial quotients is generated */
/* once on first call; it has length 16 and starts 4,3,4,3,8,3,4,3,30,3,4,3,8,3,4,3. */

#include <stdint.h>
#include <stdlib.h>

/* cast a small integer to the sequence type */
#define TOSEQ(x) ((seq_t)(x))

/* storage for the period of partial quotients of √914 */
static uint32_t *cf_period = NULL;
static size_t cf_len = 0;

/* initialise cf_period once on first call to a() */
static void init_cf_period(void)
{
    if (cf_len != 0) return;                /* already initialised */

    const uint32_t D  = 914U;               /* square‑free part */
    const uint32_t a0 = 30U;                /* floor(√914) */
    uint32_t m = 0U, d = 1U, a = a0;
    uint32_t m_next, d_next, a_next;

    size_t cap = 16;
    cf_period = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!cf_period) return;                /* allocation failed – unlikely */

    do {
        m_next = d * a - m;
        d_next = (D - m_next * m_next) / d;
        a_next = (a0 + m_next) / d_next;

        if (cf_len >= cap) {
            cap <<= 1;
            uint32_t *tmp = (uint32_t *)realloc(cf_period, cap * sizeof(uint32_t));
            if (!tmp) return;              /* allocation failed */
            cf_period = tmp;
        }
        cf_period[cf_len++] = a_next;

        m = m_next;
        d = d_next;
        a = a_next;
    } while (a_next != 2U * a0);            /* period ends when a == 2*a0 */
}

/* sequence function required by the build */
seq_t a(idx_t n)
{
    init_cf_period();          /* ensure the period is ready */

    if (n == 0U)
        return TOSEQ(1);       /* q0 = 1 */

    /* initialise the first two denominators */
    seq_t q_prevprev = TOSEQ(1);           /* q0 */
    seq_t q_prev     = TOSEQ(cf_period[0]);/* q1 */

    if (n == 1U)
        return q_prev;

    /* iterate from index 2 up to n */
    for (idx_t i = 2U; i <= n; ++i) {
        uint32_t ai = cf_period[(i - 1U) % cf_len];
        seq_t q_curr = TOSEQ(ai) * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }
    return q_prev;
}
