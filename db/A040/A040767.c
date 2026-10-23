/*
 * A040767  Continued fraction for sqrt(796).
 * https://oeis.org/A040767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040767: Continued fraction for sqrt(796) */
/* Returns the nth coefficient of the continued‑fraction expansion of sqrt(796). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sqrt_floor.h"

static seq_t *cf = NULL;          /* coefficients: cf[0] = a(0) */
static size_t cf_len = 0;         /* total stored coefficients (a(0)+period) */
static size_t period_len = 0;     /* length of the periodic part (>=1) */
static bool initialized = false;

static void init_cf(void)
{
    const idx_t D = 796;
    seq_t a0 = sqrt_floor((seq_t)D);  /* a(0) = floor(sqrt(796)) */

    seq_t m   = 0;          /* m_0 */
    seq_t d   = 1;          /* d_0 */
    seq_t a_c = a0;         /* a_0 */

    /* Allocate initial array with some capacity.  It will grow if needed. */
    size_t cap = 32;
    cf = (seq_t *)malloc(cap * sizeof(seq_t));
    cf[0] = a0;
    cf_len = 1;

    for (;;)
    {
        seq_t m_next = d * a_c - m;
        seq_t d_next = ((seq_t)D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;          /* a_{k+1} */

        /* Grow array if necessary. */
        if (cf_len == cap)
        {
            cap <<= 1;
            cf = (seq_t *)realloc(cf, cap * sizeof(seq_t));
        }
        cf[cf_len++] = a_next;

        /* The period ends when a_{k+1} == 2*a0. */
        if (a_next == (seq_t)2 * a0)
        {
            period_len = cf_len - 1;   /* exclude the initial a0 */
            break;
        }

        m = m_next;
        d = d_next;
        a_c = a_next;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_cf();
    }

    if (n == 0) {
        return cf[0];
    }

    if ((size_t)n < cf_len) {
        return cf[(size_t)n];
    }

    /* For n >= cf_len, we are in the repeating part. */
    size_t t = ((size_t)(n - 1)) % period_len;
    return cf[1 + t];
}
