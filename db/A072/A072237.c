/*
 * A072237  Smallest factorial containing exactly n 8's.
 * https://oeis.org/A072237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   digit_counts, mul_small
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
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t x, unsigned int m); */
#include "../utils/digit_counts.h"/* void digit_counts(seq_t x, unsigned char cnt[10]); */

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
