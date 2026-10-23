/*
 * A041359  Denominators of continued fraction convergents to sqrt(193).
 * https://oeis.org/A041359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent of √193 (a(0) = 1,
 *            a(1) = 1, for n ≥ 2 : a(n) = a_{n}·a(n‑1) + a(n‑2), where the
 *            coefficients a_{n} are the periodic continued‑fraction digits of
 *            √193)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses static cache to handle arbitrary‑ precision
   seq_t (unsigned integer or arbitrary‑precision class).  It works as
   both C11 and C++17 code. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Static cache of the continued‑fraction coefficients of √193. */
static uint32_t *period = NULL;   /* coefficients a1 … a_k      */
static size_t   period_len = 0;   /* number of coefficients      */

/* Static cache of denominators q_n. */
static seq_t   *denoms  = NULL;   /* q_n values                 */
static size_t   den_cap = 0;      /* allocated capacity         */
static size_t   den_len = 0;      /* number of computed values  */
static int      initialized = 0;  /* 1 when everything is set   */

/* Compute the period of √193 and store it in the static arrays.  */
static void init_period(void)
{
    const uint32_t D  = 193;      /* square‑free part */
    const uint32_t a0 = 13;       /* floor(√193)      */
    uint64_t m = 0, d = 1, a = a0;

    /* Temporary buffer – period length is guaranteed < 50 for D≤1000. */
    uint32_t tmp[64];
    size_t i = 0;

    while (1) {
        uint64_t m_next = d * a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint32_t a_next = (a0 + m_next) / d_next;

        tmp[i++] = a_next;                /* a_{k+1} */
        if (a_next == 2 * a0)             /* last coefficient in the period */
            break;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    period_len = i;
    period = (uint32_t *)malloc(period_len * sizeof(uint32_t));
    if (!period) exit(1);
    memcpy(period, tmp, period_len * sizeof(uint32_t));
}

/* Ensure that denoms has capacity for at least 'needed' elements.  */
static void ensure_denoms_capacity(size_t needed)
{
    if (needed <= den_cap)
        return;

    size_t new_cap = den_cap ? den_cap : 4;
    while (new_cap < needed)
        new_cap <<= 1;            /* grow geometrically */

    seq_t *tmp = (seq_t *)realloc(denoms, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    denoms = tmp;
    den_cap = new_cap;
}

/* Return the denominator of the n‑th convergent of √193.   */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_period();

        den_cap = 4;
        denoms  = (seq_t *)malloc(den_cap * sizeof(seq_t));
        if (!denoms) exit(1);

        denoms[0] = (seq_t)1;          /* q0 */
        /* q1 = a1, which is the first coefficient in the period. */
        denoms[1] = (seq_t)period[0];
        den_len  = 2;
        initialized = 1;
    }

    if ((size_t)n < den_len)
        return denoms[(size_t)n];

    /* Compute all missing denominators up to n. */
    ensure_denoms_capacity((size_t)n + 1);

    for (idx_t i = (idx_t)den_len; i <= n; ++i) {
        /* a_i is the (i‑1)-th coefficient in the period (0‑based). */
        size_t coeff_idx = ((size_t)(i - 1)) % period_len;
        seq_t term = (seq_t)period[coeff_idx] * denoms[i - 1] + denoms[i - 2];
        denoms[i] = term;
    }

    den_len = (size_t)n + 1;
    return denoms[(size_t)n];
}
