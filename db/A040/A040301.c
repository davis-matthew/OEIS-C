/*
 * A040301  Continued fraction for sqrt(319).
 * https://oeis.org/A040301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction expansion of sqrt(319). The sequence a(n)
 *            gives the terms of the continued fraction. The sequence is
 *            periodic after a(0). a(0) = floor(sqrt(319)) = 17, and the
 *            period consists of the following 14 terms: 1, 6, 5, 1, 4, 3, 1,
 *            3, 4, 1, 5, 6, 1, 34
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t period[64];   /* period[0] = a(0), period[1..period_len] = a(1) ... a(k) */
static idx_t period_len;   /* number of terms in the period (excluding a(0)) */
static bool initialized = false;

static void init_period(void)
{
    const idx_t D = 319;
    /* Compute floor(sqrt(D)) without floating‑point arithmetic. */
    idx_t a0 = 0;
    idx_t low = 0, high = D;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq <= D) {
            a0 = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    period[0] = (seq_t)a0;

    idx_t m = 0, d = 1, aPrev = a0;
    idx_t count = 0;                /* number of stored period terms after a(0) */

    while (1) {
        m = d * aPrev - m;
        d = (D - m * m) / d;
        aPrev = (a0 + m) / d;
        period[1 + count] = (seq_t)aPrev;
        count++;
        if (aPrev == 2 * a0)       /* end of the period */
            break;
    }

    period_len = count;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_period();
    if (n == 0) return period[0];
    idx_t idx = (n - 1) % period_len;   /* 0‑based index in the period */
    return period[1 + idx];
}
