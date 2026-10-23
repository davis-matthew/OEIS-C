/*
 * A048572  a(n) = sum of digits of a(n-1)*a(n-2).
 * https://oeis.org/A048572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   a(1) = 1, a(2) = 2, * a(n) = digit_sum(a(n-1) * a(n-2)) (n >= 3)
 *            * Periodic with period 24 for n >= 25
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

static seq_t cycle[24];
static bool initialized = false;

static void init_cycle(void)
{
    cycle[0] = 1;   /* a(1) */
    cycle[1] = 2;   /* a(2) */
    for (idx_t i = 3; i <= 24; ++i) {          /* compute a(3) … a(24) */
        seq_t prod = cycle[i-3] * cycle[i-2];   /* a(n-2) * a(n-1) */
        cycle[i-1] = digit_sum(prod);          /* a(n)   */
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_cycle();

    if (n <= 24)
        return cycle[(size_t)(n - 1)];

    idx_t k = (n - 1) % 24;                     /* 0‑based index in the cycle */
    return cycle[(size_t)k];
}
