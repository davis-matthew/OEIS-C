/*
 * A200795  Number of n X 2 0..3 arrays with values 0..3 introduced in row
 *          major order and each element equal to no more than two horizontal
 *          and vertical neighbors.
 * https://oeis.org/A200795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A200795
   Empirical recurrence (valid for n > 10):
     a(n) = 18*a(n-1) - 35*a(n-2) - 48*a(n-3) - 11*a(n-4)
            + 90*a(n-5) + 36*a(n-6) - 12*a(n-7) - 36*a(n-8)
   Base terms (for n = 1 … 10):
     2, 15, 178, 2614, 40148, 622645, 9676364, 150442627, 2339207390, 36372631268
*/

#include <stddef.h>
#include <stdbool.h>

#pragma once

static const seq_t BASE[10] = {
    (seq_t)2ULL,
    (seq_t)15ULL,
    (seq_t)178ULL,
    (seq_t)2614ULL,
    (seq_t)40148ULL,
    (seq_t)622645ULL,
    (seq_t)9676364ULL,
    (seq_t)150442627ULL,
    (seq_t)2339207390ULL,
    (seq_t)36372631268ULL
};

/* Compute a(n) for n >= 1.  The function works for any seq_t type:
   unsigned 8/16/32/64/128 or an arbitrary‑precision integer class.
   For seq_t types that are unsigned, the recurrence is valid for all n
   whose values fit in seq_t.  For larger n the result may wrap, but
   the specification only requires correctness for values that fit. */
seq_t a(idx_t n)
{
    if (n <= (idx_t)10)
        return BASE[n - 1];

    /* last eight terms: t[0] = a(k), t[1] = a(k-1), ..., t[7] = a(k-7) */
    seq_t t[8];
    t[0] = BASE[9];  /* a(10) */
    t[1] = BASE[8];  /* a(9) */
    t[2] = BASE[7];  /* a(8) */
    t[3] = BASE[6];  /* a(7) */
    t[4] = BASE[5];  /* a(6) */
    t[5] = BASE[4];  /* a(5) */
    t[6] = BASE[3];  /* a(4) */
    t[7] = BASE[2];  /* a(3) */

    seq_t result = 0;
    idx_t i;
    for (i = 11; i <= n; ++i) {
        seq_t pos = t[0] * (seq_t)18 + t[4] * (seq_t)90 + t[5] * (seq_t)36;
        seq_t neg = t[1] * (seq_t)35 + t[2] * (seq_t)48
                    + t[3] * (seq_t)11 + t[6] * (seq_t)12 + t[7] * (seq_t)36;
        result = pos - neg;  /* pos >= neg for all terms that fit */

        /* shift the circular buffer */
        t[7] = t[6];
        t[6] = t[5];
        t[5] = t[4];
        t[4] = t[3];
        t[3] = t[2];
        t[2] = t[1];
        t[1] = t[0];
        t[0] = result;
    }

    return result;
}
