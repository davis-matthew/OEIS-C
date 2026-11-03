/*
 * A040962  Continued fraction for sqrt(994).
 * https://oeis.org/A040962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(994) */
/* a(0) = floor(sqrt(994)) = 31 */
/* a(n) for n>=1 is the (n-1)‑th element of the repeating period [1,1,8,1,1,62] */
#include <stddef.h>
#include <stdbool.h>

/* Compute the period of the continued fraction for sqrt(994) and store it
   in the provided buffer.  The buffer must be large enough for the period
   (which here is 6).  The function fills *len with the period length
   and returns the leading term a0. */
static void compute_period(seq_t *period, size_t *len, seq_t *a0)
{
    const seq_t D = (seq_t)994;
    seq_t m = 0;
    seq_t d = 1;
    *a0 = (seq_t)31;                 /* floor(sqrt(994)) */
    seq_t a = *a0;
    size_t i = 0;

    /* The period ends when a becomes 2*a0.  We store each a in period[] */
    do {
        m = d * a - m;
        d = (D - m * m) / d;          /* exact division */
        a = (*a0 + m) / d;
        period[i++] = a;
    } while (a != 2 * (*a0));

    *len = i;
}

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t a0;
    static seq_t period[16];
    static size_t period_len;

    if (!initialized) {
        compute_period(period, &period_len, &a0);
        initialized = true;
    }

    if (n == 0)
        return a0;

    size_t idx = (size_t)(n - 1) % period_len;
    return period[idx];
}
