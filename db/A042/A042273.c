/*
 * A042273  Denominators of continued fraction convergents to sqrt(662).
 * https://oeis.org/A042273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent of sqrt(662)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* constants for sqrt(662) */
static const uint64_t D = 662u;
static const uint64_t A0 = 25u;   /* floor(sqrt(662)) */

/* cache for the sequence values */
static seq_t *denoms = NULL;
static size_t denoms_cap = 0;
static idx_t denoms_size = 0;

/* state for continued‑fraction generation */
static bool initialized = false;
static seq_t p_nm2, p_nm1;        /* previous two numerators */
static seq_t q_nm2, q_nm1;        /* previous two denominators */
static uint64_t m, d, a_cur;     /* algorithmic variables */

/* enlarge cache if needed */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (denoms_cap >= needed)
        return;
    size_t new_cap = denoms_cap ? denoms_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    denoms = (seq_t *)realloc(denoms, new_cap * sizeof(seq_t));
    denoms_cap = new_cap;
}

/* initialise the first convergent (index 0) and compute a(1) */
static void init_state(void)
{
    /* q(-1) = 0, q(0) = 1
       p(-1) = 1, p(0) = a0 */
    p_nm2 = (seq_t)1;
    p_nm1 = (seq_t)A0;
    q_nm2 = (seq_t)0;
    q_nm1 = (seq_t)1;

    denoms[0] = (seq_t)1;          /* denominator for index 0 */
    denoms_size = 1;

    /* prepare first partial quotient a1 */
    m = 0;
    d = 1;
    uint64_t m_next = d * A0 - m;           /* = a0 */
    uint64_t d_next = (D - m_next * m_next) / d;
    uint64_t a_next = (A0 + m_next) / d_next;
    m = m_next;
    d = d_next;
    a_cur = a_next;
    initialized = true;
}

/* retrieve the denumerator for the n-th convergent */
seq_t a(idx_t n)
{
    if (!initialized)
    {
        denoms_cap = 16;
        denoms = (seq_t *)malloc(denoms_cap * sizeof(seq_t));
        init_state();
    }

    if (n < denoms_size)
        return denoms[n];

    ensure_capacity(n);

    while (denoms_size <= n)
    {
        /* use current partial quotient a_cur */
        seq_t a_seq = (seq_t)a_cur;
        seq_t p_k = a_seq * p_nm1 + p_nm2;
        seq_t q_k = a_seq * q_nm1 + q_nm2;

        denoms[denoms_size++] = q_k;

        /* shift state for next iteration */
        p_nm2 = p_nm1;
        p_nm1 = p_k;
        q_nm2 = q_nm1;
        q_nm1 = q_k;

        /* compute next partial quotient */
        uint64_t m_next = d * a_cur - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (A0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return denoms[n];
}
