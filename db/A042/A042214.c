/*
 * A042214  Numerators of continued fraction convergents to sqrt(633).
 * https://oeis.org/A042214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued‐fraction convergents to sqrt(633) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* local state ------------------------------------------------------------ */
static uint64_t *period = NULL;      /* periodic part of the CF of sqrt(633) */
static size_t   period_len = 0;       /* number of entries in period       */
static bool     period_initialized = false;

/* compute the periodic part of the continued fraction of sqrt(633) */
static void init_period(void)
{
    const uint64_t D  = 633;
    const uint64_t a0 = 25;          /* floor(sqrt(633))           */
    uint64_t m = 0, d = 1, a = a0;

    size_t cap = 64;
    period = (uint64_t *)malloc(cap * sizeof(uint64_t));
    period_len = 0;

    do {
        /* next m,d,a : standard quadratic‐form algorithm      */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* grow the array if needed */
        if (period_len == cap) {
            cap <<= 1;
            period = (uint64_t *)realloc(period, cap * sizeof(uint64_t));
        }

        period[period_len++] = a;   /* store the periodic term            */
    } while (a != 2 * a0);           /* termination condition for square roots */

    period_initialized = true;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!period_initialized)
        init_period();

    if (n == 0)                      /* a(0) = floor(sqrt(633)) = 25   */
        return (seq_t)25;

    /* recurrence for convergents:
       p_{-1} = 1, p_0 = a0 = 25
       p_k   = a_k * p_{k-1} + p_{k-2}                         */
    seq_t prev2 = (seq_t)1;          /* p_{k-2}  */
    seq_t prev1 = (seq_t)25;         /* p_{k-1}  */
    seq_t cur   = 0;                 /* p_k      */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ai = (seq_t)period[(i - 1) % period_len];
        cur = ai * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;                      /* p_n  */
}
