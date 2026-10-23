/*
 * A041695  Denominators of continued fraction convergents to sqrt(367).
 * https://oeis.org/A041695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=6, and for n≥2, a(n)=c_n*a(n-1)+a(n-2) where the
 *            coefficients c_n are those of the continued‑fraction expansion
 *            of √367. The first coefficient is a0=19; the remaining
 *            coefficients repeat with period – the loop below computes them
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 73,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

static bool coeffs_inited = false;
static unsigned int *cf = NULL;
static size_t cf_len  = 0;
static size_t cf_cap  = 0;
static const unsigned int a0 = 19;
static const idx_t D_val = 367;

static void init_coeffs(void)
{
    if (coeffs_inited)
        return;
    coeffs_inited = true;

    cf_cap = 16;
    cf = (unsigned int *)malloc(cf_cap * sizeof(unsigned int));
    cf_len = 0;

    int64_t m   = 0;                /* m0   */
    int64_t d   = 1;                /* d0   */
    int64_t a   = a0;               /* a0   */

    while (1) {
        int64_t m_next = d * a - m;
        int64_t d_next = ((int64_t)D_val - m_next * m_next) / d;
        int64_t a_next = (a0 + m_next) / d_next;

        if (cf_len == cf_cap) {
            cf_cap <<= 1;
            cf = (unsigned int *)realloc(cf, cf_cap * sizeof(unsigned int));
        }
        cf[cf_len++] = (unsigned int)a_next;

        if (a_next == 2 * a0) break;
        m = m_next;
        d = d_next;
        a = a_next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    init_coeffs();

    seq_t prevprev = 1;  /* q_{-2} */
    seq_t prev     = 0;  /* q_{-1} */
    seq_t qk = 0;

    for (idx_t k = 0; k <= n; ++k) {
        unsigned int ak = (k == 0) ? a0 : cf[(k - 1) % cf_len];
        seq_t prod = mul_small(prev, ak);
        qk = prod + prevprev;
        prevprev = prev;
        prev = qk;
    }
    return prev;
}
