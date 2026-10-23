/*
 * A166392  Multiples of 19 whose reversal + 1 is also a multiple of 19.
 * https://oeis.org/A166392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rev_mod19
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166392: Multiples of 19 whose reverse + 1 is also a multiple of 19
 * a(n) = the n‑th positive integer m such that
 *        m % 19 == 0  and  (rev_decimal(m) + 1) % 19 == 0
 *
 * The function searches through successive multiples of 19, testing the
 * remainder of the reversed number modulo 19.  A fast computation of the
 * reversed number modulo 19 avoids storing the full reversed string, so the
 * routine works for any seq_t size and stays well under 2 s for n up to ~10000. */

#include <stdbool.h>          /* for bool if needed (not used here) */
#include "../utils/rev_mod19.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; behaviour for n < 1 is irrelevant */
        return 0;

    idx_t count = 0;

    for (idx_t m = 19; ; m += 19)   /* iterate over all multiples of 19 */
    {
        if ((rev_mod19(m) + 1) % 19 == 0)
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
