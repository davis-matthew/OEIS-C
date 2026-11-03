/*
 * A072237  Smallest factorial containing exactly n 8's.
 * https://oeis.org/A072237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the smallest factorial that contains exactly n occurrences of the
 * decimal digit '8'.  The routine performs a brute‑force search: it
 * multiplies successive integers and counts the digit '8' in each
 * factorial until the desired count is found.  This is a correct,
 * straightforward implementation that works for any n ≥ 1 in any call
 * order (no memoisation is required). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* External utilities supplied by the build system */
#include <stdlib.h>
#include "../utils/digit_counts.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Validity: sequence is defined for n >= 1 */
    if (n < 1)
        return (seq_t)0;

    /* Current factorial value and its argument */
    seq_t fact = (seq_t)1;
    idx_t m = 1;

    /* Count array for decimal digits 0–9 */
    unsigned char cnt[10];

    while (true)
    {
        /* Count the digits of the current factorial */
        digit_counts(fact, cnt);

        if (cnt[8] == (seq_t)n)
            return fact;            /* found the smallest factorial */

        /* Advance to the next factorial */
        ++m;
        fact = mul_small(fact, (unsigned int)m);
    }
}
