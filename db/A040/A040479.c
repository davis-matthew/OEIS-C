/*
 * A040479  Continued fraction for sqrt(502).
 * https://oeis.org/A040479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040479: continued fraction for sqrt(502) */
/* a(0) = 22, a(n>0) are the terms of the simple continued fraction of sqrt(502) */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"

static seq_t *period = NULL;           /* array of period terms */
static size_t period_len = 0;          /* number of terms stored */

/* Initialise the period of the continued fraction once */
static void init_period(void)
{
    if (period != NULL)                /* already initialised */
        return;

    const idx_t D   = 502;             /* radicand */
    const idx_t a0  = 22;              /* floor(sqrt(502)) */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;                      /* current a_k value */

    /* Allocate space for the period; 64 is more than enough for D = 502 */
    period = seq_alloc_new_array(64);
    period_len = 0;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        period[period_len++] = (seq_t)a;

        if (a == 2 * a0)   /* end of period for sqrt(D) */
            break;
    }
}

/* Return the n-th term of the continued fraction of sqrt(502) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)22;     /* a(0) is the integer part */

    init_period();

    /* After a(0), terms repeat the period */
    idx_t idx = (n - 1) % (idx_t)period_len;
    return period[(size_t)idx];
}
