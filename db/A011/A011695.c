/*
 * A011695  A binary m-sequence: expansion of reciprocal of
 *          x^8+x^7+x^6+x^5+x^4+x^2+1.
 * https://oeis.org/A011695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) ^ a(n-7) ^ a(n-6) ^ a(n-5) ^ a(n-4) ^ a(n-2) for n
 *            ≥ 8, with initial values a(0) = 0, a(1) = 0, a(2) = 0, a(3) = 0,
 *            a(4) = 0, a(5) = 0, a(6) = 0, a(7) = 1. The sequence is
 *            255‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD 255

static seq_t period_cache[PERIOD];
static bool period_initialized = false;

seq_t a(idx_t n)
{
    if (!period_initialized)
    {
        /* initialise the first 8 terms */
        period_cache[0] = 0;
        period_cache[1] = 0;
        period_cache[2] = 0;
        period_cache[3] = 0;
        period_cache[4] = 0;
        period_cache[5] = 0;
        period_cache[6] = 0;
        period_cache[7] = 1;

        /* compute the remaining 247 terms of the period */
        for (size_t i = 8; i < PERIOD; ++i)
        {
            period_cache[i] =
                period_cache[i-8] ^ period_cache[i-7] ^
                period_cache[i-6] ^ period_cache[i-5] ^
                period_cache[i-4] ^ period_cache[i-2];
        }
        period_initialized = true;
    }

    /* take the index modulo the period */
    size_t idx = (size_t)(n % PERIOD);
    return period_cache[idx];
}
