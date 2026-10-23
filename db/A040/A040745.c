/*
 * A040745  Continued fraction for sqrt(773).
 * https://oeis.org/A040745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   terms of the continued fraction expansion of sqrt(773)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   integer_sqrt773
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/integer_sqrt773.h"

/* Global cache for the period of the continued fraction of sqrt(773). */
static seq_t a0_seq = 0;                     /* integer part of sqrt(773) */
static seq_t period[60];                    /* period terms (max 60 for D = 773) */
static size_t period_len = 0;               /* actual length of the period */
static bool initialized = false;

/* Initialise the period cache.  The standard algorithm for continued
   fractions of quadratic irrationals is used.  All intermediate
   arithmetic is performed with 64‑bit unsigned integers to avoid
   overflow on narrow seq_t types. */
static void init_period(void)
{
    if (initialized) return;

    const uint64_t D = 773ULL;
    uint64_t sqrt = integer_sqrt773();          /* floor(sqrt(773)) == 27 */
    a0_seq = (seq_t)sqrt;

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = sqrt;
    period_len = 0;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (sqrt + m) / d;
        period[period_len++] = (seq_t)a;        /* a <= 54, fits in seq_t */
        if (a == 2 * sqrt) break;               /* period terminates here */
    }

    initialized = true;
}

/* Return the n-th term of the sequence (a(0) = floor(sqrt(773)), a(n>0)
   are the periodic continued‑fraction coefficients). */
seq_t a(idx_t n)
{
    init_period();                /* ensure period is cached */
    if (n == 0) return a0_seq;
    return period[(size_t)((n - 1) % period_len)];
}
