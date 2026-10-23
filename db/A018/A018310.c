/*
 * A018310  Divisors of 160.
 * https://oeis.org/A018310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 160 in increasing order */
#include <stdbool.h>

static seq_t cache[12];
static bool initialized = false;
static void init_cache(void)
{
    idx_t pos = 0;
    for (idx_t d = 1; d <= 160; ++d) {
        if (160 % d == 0) {
            cache[pos++] = (seq_t)d;   /* store divisor */
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    /* 1‑based index */
    if (!initialized) init_cache();
    if (n == 0) return (seq_t)0;        /* outside defined range */
    if (n <= 12) return cache[n - 1];   /* valid term */
    return (seq_t)0;                    /* undefined beyond 12 */
}
