/*
 * A042158  Numerators of continued fraction convergents to sqrt(604).
 * https://oeis.org/A042158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(604) */

#include <stdint.h>
#include <stdlib.h>

/* periodic continued‑fraction coefficients of sqrt(604) */
static seq_t _cf_period[64];
static size_t _cf_len = 0;

/* build the periodic part once */
static void _init_cf(void)
{
    if (_cf_len != 0)
        return;

    const idx_t D   = 604;          /* square‑free part */
    const idx_t a0  = 24;           /* floor(sqrt(D)) */
    idx_t m = 0, d = 1, a = a0;

    /* generate coefficients until the last one equals 2*a0 (48) */
    do
    {
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        _cf_period[_cf_len++] = (seq_t)a_next;

        if (a_next == 2 * a0)
            break;

        m = m_next;
        d = d_next;
        a = a_next;
    } while (1);
}

/* coefficient a_k (k>=0) */
static seq_t _cf_coeff(idx_t k)
{
    if (k == 0)
        return (seq_t)24;          /* a_0 = floor(sqrt(604)) */
    _init_cf();
    return _cf_period[(k - 1) % _cf_len];
}

/* main sequence function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)24;          /* a(0) = 24 */

    seq_t prev2 = (seq_t)1;        /* p_{-1} */
    seq_t prev1 = (seq_t)24;       /* p_0      */
    _init_cf();

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t coeff = _cf_coeff(k);
        seq_t cur   = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
