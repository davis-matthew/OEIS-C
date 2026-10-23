/*
 * A041518  Numerators of continued fraction convergents to sqrt(276).
 * https://oeis.org/A041518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A041518
    Numerators of the convergents to sqrt(276).
    Recurrence:
        p_-2 = 0,
        p_-1 = 1,
        p_n  = q_n * p_(n-1) + p_(n-2)   (n ≥ 0)
    where q_0 = floor(sqrt(276)) = 16, and for n ≥ 1
    q_n is the n‑th partial quotient of the continued fraction
    of sqrt(276).  The continued fraction is
        sqrt(276) = [16; 1,1,1,1,2,2,2,1,1,1,1,1,32] repeating.
*/

#include <stdlib.h>
#include <stdbool.h>

/* One–time storage of the periodic part of the continued fraction. */
static seq_t *period = NULL;     /* a_1 … a_period_len */
static size_t period_len = 0;
static bool period_inited = false;

/* Integer square root of a small positive number (floor). */
static int int_sqrt(int n)
{
    int s = 0;
    while ((s + 1) * (s + 1) <= n) ++s;
    return s;
}

/* Compute the period of the continued fraction of sqrt(276) once. */
static void init_period(void)
{
    if (period_inited) return;

    const int D = 276;
    const int a0 = int_sqrt(D);          /* floor(sqrt(276)) = 16 */

    int m = 0;
    int d = 1;
    int a = a0;

    /* dynamic array for the periodic partial quotients a_1, a_2, … */
    size_t cap = 8;
    period = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t len = 0;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (len == cap) {
            cap <<= 1;
            period = (seq_t *)realloc(period, cap * sizeof(seq_t));
        }
        period[len++] = (seq_t)a;

        if (d == 1)      /* period ends when d == 1 */
            break;
    }

    period_len = len;
    period_inited = true;
}

/* -------------------------------------------------------------
 *  Public function: a(idx_t n)
 * ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_period();

    const seq_t a0 = (seq_t)16;          /* floor(sqrt(276)) */

    seq_t p_prev2 = 0;   /* p_-2 */
    seq_t p_prev  = 1;   /* p_-1 */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t q;
        if (i == 0) {
            q = a0;
        } else {
            size_t idx = (size_t)((i - 1) % period_len);
            q = period[idx];
        }
        seq_t p = q * p_prev + p_prev2;
        p_prev2 = p_prev;
        p_prev  = p;
    }

    return p_prev;   /* p_n */
}
