/*
 * A040957  Continued fraction for sqrt(989).
 * https://oeis.org/A040957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of √989
   a(0) = floor(sqrt(989)) = 31.
   For n > 0, a(n) = the (n‑th) term of the periodic continued‑fraction
   expansion of sqrt(989).  The period is generated once by the standard
   algorithm for quadratic irrationals.  The sequence is memoised so the
   same period is reused for subsequent calls. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/* The start of the period (a0) */
static const seq_t a0 = 31;

/* Storage for one full period, excluding a0. */
static seq_t *period = NULL;      /* array holding the period terms */
static size_t period_len = 0;      /* actual number of stored terms */

/* Initialise the period on first use.  The algorithm keeps the previous
   a_k value (prev_a) to compute the next m and d.  The period terminates
   when the next a equals 2*a0. */
static void init_period(void)
{
    if (period_len > 0)              /* already initialised */
        return;

    const seq_t D = 989;            /* the integer under the square root */

    seq_t m = 0;                    /* current m_k */
    seq_t d = 1;                    /* current d_k */
    seq_t prev_a = a0;              /* a_k, starting with a0 */
    seq_t a;                        /* a_{k+1} */

    /* Allocate an initial buffer; the period for sqrt(989) has length 32. */
    size_t cap = 32;                /* an upper bound is fine */
    period = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!period)
        return; /* out of memory; caller will get garbage */
    period_len = 0;

    while (1) {
        /* m_{k+1} = d_k * a_k - m_k */
        m = d * prev_a - m;

        /* d_{k+1} = (D - m_{k+1}^2) / d_k */
        /* m*m is guaranteed to be ≤ D, so subtraction is safe. */
        d = (D - m * m) / d;

        /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
        a = (a0 + m) / d;

        /* Store the new term in the period array. */
        if (period_len == cap) {
            cap <<= 1;                      /* double the buffer */
            seq_t *tmp = (seq_t *)realloc(period, cap * sizeof(seq_t));
            if (!tmp) { free(period); period = NULL; period_len = 0; return; }
            period = tmp;
        }
        period[period_len++] = a;

        /* Terminate on the final term of the period (2*a0). */
        if (a == a0 * 2)
            break;

        prev_a = a;      /* advance to the next a_k */
    }
}

seq_t a(idx_t n)
{
    init_period();

    if (n == 0)            /* the initial term */
        return a0;

    /* For n ≥ 1, use periodicity: a(n) = period[(n‑1) mod period_len] */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
