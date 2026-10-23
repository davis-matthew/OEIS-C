/*
 * A040758  Continued fraction for sqrt(787).
 * https://oeis.org/A040758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction for sqrt(787) – a periodic continued fraction
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Periodic part of the continued fraction, not including the first term a(0).  */
static seq_t period[256];
static size_t period_len = 0;
static seq_t a0 = 0;
static bool init_done = false;

/* Initialize the periodic data once.  */
static void init_seq(void)
{
    /* Compute a0 = floor(sqrt(787)) using integer arithmetic.  */
    idx_t D = 787;
    idx_t k = 0;
    while ((k + 1) * (k + 1) <= D)
        ++k;
    a0 = (seq_t)k;            /* a0 fits in idx_t, so the cast is safe.  */

    /* Compute the periodic part of the continued fraction.  */
    idx_t m = 0;          /* numerator of fraction decomposition */
    idx_t d = 1;          /* denominator of fraction decomposition */
    idx_t a = (idx_t)a0;  /* current term of the continued fraction */

    do
    {
        m = d * a - m;
        /* d divides (D - m*m) exactly, so the division is exact.  */
        d = (D - m * m) / d;
        a = ( (idx_t)a0 + m ) / d;
        period[period_len++] = (seq_t)a;
    } while (d != 1);      /* stop when we return to the initial denominator */
    init_done = true;
}

/* Return the n-th term of the sequence, with a(0) = floor(sqrt(787)). */
seq_t a(idx_t n)
{
    if (!init_done)
        init_seq();

    if (n == 0)
        return a0;

    /* The periodic part repeats with length period_len.  */
    size_t pos = (size_t)((n - 1) % (idx_t)period_len);
    return period[pos];
}
