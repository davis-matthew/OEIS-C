/*
 * A040751  Continued fraction for sqrt(779).
 * https://oeis.org/A040751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction for √779 – the sequence of the periodic part
 *            after the integer part a0. a(0) = ⌊√779⌋ = 27, and for n≥1 a(n)
 *            = periodic_term[(n−1) mod period_length]
 * Range:     u8 none, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

static seq_t a0_val;               /* a(0) */
static seq_t period[512];          /* periodic terms a(1), a(2), … */
static size_t period_len = 0;      /* number of periodic terms */

/* Compute a0 and the period using the standard continued‑fraction algorithm */
static void compute_period(void)
{
    const seq_t D = (seq_t)779;
    a0_val = floor_sqrt(D);

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_curr = a0_val;

    /* Generate terms until the denominator becomes 1 (the period ends) */
    do {
        m = d * a_curr - m;
        d = (D - m * m) / d;
        a_curr = (a0_val + m) / d;
        period[period_len++] = a_curr;
    } while (d != 1);
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        compute_period();
        initialized = true;
    }

    if (n == 0)
        return a0_val;

    return period[(size_t)((n - 1) % period_len)];
}
