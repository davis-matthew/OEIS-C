/*
 * A041862  Numerators of continued fraction convergents to sqrt(453).
 * https://oeis.org/A041862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(0) = 21 Let c[1..10] = {3,1,1,10,14,10,1,1,3,42} (periodic
 *            with c[10] = 2*21). For n ≥ 1: a(n) = c[(n‑1) % 10] * a(n‑1) +
 *            a(n‑2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const seq_t A0 = (seq_t)21;
#define PERIOD 10
static const uint32_t PERIODIC[PERIOD] = {3U,1U,1U,10U,14U,10U,1U,1U,3U,42U};

seq_t a(idx_t n)
{
    if (n == 0)            /* the first term */
        return A0;

    seq_t p_prev2 = A0;    /* a(0) */
    seq_t p_prev  = (seq_t)PERIODIC[0] * p_prev2 + (seq_t)1; /* a(1) */

    if (n == 1)
        return p_prev;

    for (idx_t i = 2; i <= n; ++i) {
        uint32_t ai = PERIODIC[(size_t)((i - 1) % PERIOD)];
        seq_t p_curr = (seq_t)ai * p_prev + p_prev2;
        p_prev2 = p_prev;
        p_prev  = p_curr;
    }
    return p_prev;
}
