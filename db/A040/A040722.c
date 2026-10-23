/*
 * A040722  Continued fraction for sqrt(750).
 * https://oeis.org/A040722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(750).  
   Computed by the standard square‑root continued fraction algorithm. */

#include <stdlib.h>   /* abort, realloc, size_t */

static seq_t *period = NULL;   /* array of periodic terms a(1..period_len) */
static size_t period_len = 0;  /* length of the period (16 for sqrt(750)) */
static seq_t a0_seq = 0;       /* a(0) = floor(sqrt(750)) = 27 */
static idx_t a0_idx = 0;       /* same value kept as idx_t for the algorithm */

static void init_cf(void)
{
    if (period != NULL) return;          /* already initialized */

    /* compute a0 = floor(sqrt(750)) with integer arithmetic */
    const idx_t D = 750;
    idx_t lim = 0;
    while ((lim + 1) * (lim + 1) <= D) ++lim;
    a0_idx  = lim;
    a0_seq  = (seq_t)lim;               /* same numeric value */

    /* build the period by the standard continued fraction algorithm */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0_idx;
    const idx_t two_a0 = 2 * a0_idx;

    period_len = 0;
    period = NULL;

    do {
        /* next values in the algorithm */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0_idx + m) / d;

        /* append the new period term */
        seq_t *tmp = (seq_t *)realloc(period, (period_len + 1) * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        period = tmp;
        period[period_len++] = (seq_t)a;
    } while (a != two_a0);               /* stop when a == 2*a0; that term is part of the period */
}

seq_t a(idx_t n)
{
    init_cf();                          /* ensure the period is ready */

    if (n == 0) return a0_seq;          /* a(0) */

    /* subsequent terms repeat with the computed period */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
