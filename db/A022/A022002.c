/*
 * A022002  Decimal expansion of 1/998.
 * https://oeis.org/A022002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   decimal expansion of 1/998 (a(n) = digit at position n of
 *            0.001002004008...)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD 498

static seq_t period[PERIOD];
static bool period_initialized = false;

static void init_period(void)
{
    seq_t rem = 1;
    for (idx_t i = 0; i < PERIOD; ++i) {
        rem *= 10;
        period[i] = rem / 998;
        rem = rem % 998;
    }
    period_initialized = true;
}

seq_t a(idx_t n)
{
    /* Return the n‑th decimal digit of 1/998 (0‑based). */
    if (!period_initialized)
        init_period();

    return period[(size_t)(n % PERIOD)];
}
