/*
 * A040943  Continued fraction for sqrt(975).
 * https://oeis.org/A040943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(975). */
/* a(0) = floor(sqrt(975)); the following terms repeat the
   periodic part of the continued fraction expansion. */
#include <stdbool.h>
#include <stdint.h>

/* Integer square root for unsigned 64‑bit values. */
static idx_t integer_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Static data for the continued‑fraction period. */
static bool period_initialized = false;
static idx_t period_len = 0;
static seq_t period[256];
static seq_t a0;

/* Compute the periodic part once. */
static void init_period(void)
{
    if (period_initialized)
        return;

    const idx_t D = 975;
    idx_t a0_i = integer_sqrt(D);
    a0 = (seq_t)a0_i;          /* cast to seq_t (works for built‑in and big‑int) */

    idx_t m = 0 /* (idx_t) */, d = 1 /* (idx_t) */, a = a0_i;
    period_len = 0;

    while (true) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0_i + m) / d;
        period[period_len++] = (seq_t)a;   /* store term as seq_t */

        /* The period ends when the term equals 2*a0_i. */
        if (a == 2 * a0_i)
            break;
    }

    period_initialized = true;
}

/* Return the n-th term, with a(0) the integer part of sqrt(975). */
seq_t a(idx_t n)
{
    init_period();

    if (n == 0)
        return a0;

    idx_t pos = (n - 1) % period_len;
    return period[pos];
}
