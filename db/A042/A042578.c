/*
 * A042578  Numerators of continued fraction convergents to sqrt(818).
 * https://oeis.org/A042578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 200 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042578: Numerators of continued fraction convergents to sqrt(818).
   Computed by recurrence
        p_{-2} = 0,   p_{-1} = 1,
        p_k   = a_k * p_{k-1} + p_{k-2}   (k ≥ 0),
   where a_0 = ⌊√818⌋ = 28 and the subsequent partial quotients a_k
   form the periodic cycle [1,1,1,1,56] (period 5). */

#include <stddef.h>
#include <stdint.h>

static void init_cycle(int *cycle, size_t *period)
{
    static int a0 = 0;
    static int initialized = 0;
    if (initialized) return;
    /* Integer square root of 818 */
    a0 = 28;   /* floor(sqrt(818)) */
    const int D = 818;
    int m = 0;
    int d = 1;
    int a = a0;
    size_t p = 0;
    while (1)
    {
        /* compute next m, d, a */
        int m1 = d * a - m;
        int d1 = (D - m1 * m1) / d;
        int a1 = (a0 + m1) / d1;   /* floor division, exact for integer */
        cycle[p++] = a1;
        if (d1 == 1 && a1 == 2 * a0)   /* end of period for sqrt(D) */
            break;
        m = m1;
        d = d1;
        a = a1;
    }
    *period = p;
    initialized = 1;
}

seq_t a(idx_t n)
{
    /* Precompute periodic part once */
    static int cycle[256];
    static size_t period = 0;
    if (period == 0)
        init_cycle(cycle, &period);

    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */
    seq_t p_cur;
    const seq_t a0 = 28; /* floor(sqrt(818)) */

    if (n == 0)
        return a0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t a_k;
        if (k == 0)
            a_k = a0;
        else
            a_k = (seq_t)cycle[(size_t)(k - 1) % period];

        p_cur = a_k * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }
    return p_cur;
}
