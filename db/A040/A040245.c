/*
 * A040245  Continued fraction for sqrt(262).
 * https://oeis.org/A040245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the n-th partial quotient of the simple continued
 *            fraction expansion of sqrt(262). a(0) = 16, and the sequence
 *            thereafter is periodic with period 14
 * Range:     u8 n <= 0, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Static storage for the period of the continued fraction (after a(0)). */
static bool period_computed = false;
static seq_t period[32];   /* large enough to hold the period */
static size_t period_len = 0;

/* Compute the period of sqrt(262) once. */
static void compute_period(void)
{
    if (period_computed) return;

    const seq_t D   = (seq_t)262;     /* The value under the square root */
    const seq_t a0  = (seq_t)16;      /* floor(sqrt(262)) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;
    size_t idx = 0;

    while (1) {
        /* Next values in the continued‑fraction algorithm */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        period[idx++] = a;

        /* Period ends when a equals 2 * a0 (the case for sqrt). */
        if (a == 2 * a0) {
            period_len = idx;
            period_computed = true;
            break;
        }
    }
}

/* Public function required by the compiler. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)16;        /* a(0) = floor(sqrt(262)) */

    if (!period_computed)
        compute_period();

    size_t idx = (size_t)(n - 1) % period_len;
    return period[idx];
}
