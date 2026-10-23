/*
 * A040527  Continued fraction for sqrt(551).
 * https://oeis.org/A040527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=23, a(1)=2, a(2)=8, a(3)=1, a(4)=8, a(5)=2, a(6)=46, and
 *            a(n)=a(n-6) for n≥6 (periodic continued fraction for √551)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Periodic block of continued‑fraction coefficients for √551. */
static seq_t period[6];
static bool period_initialized = false;

static void init_period(void)
{
    /* Use idx_t for all intermediary arithmetic to avoid overflow
       when seq_t is small (e.g., uint8_t). */
    const idx_t D   = (idx_t)551;   /* the radicand */
    const idx_t a0  = (idx_t)23;    /* floor(√551)     */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;
    idx_t i = 0;

    /* The standard continued‑fraction algorithm for quadratic irrationals. */
    for (;;)
    {
        /* Advance to the next Legendre variables. */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        period[(size_t)i++] = (seq_t)a;   /* store the k‑th coefficient */

        /* Termination when a == 2*a0 marks end of the period. */
        if (a == 2 * a0)
            break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;   /* a(0) = floor(√551) */

    if (!period_initialized)
    {
        init_period();
        period_initialized = true;
    }

    /* For n ≥ 1: a(n) = period[(n-1) mod 6] */
    const idx_t k = (n - 1) % (idx_t)6;
    return period[(size_t)k];
}
