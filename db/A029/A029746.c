/*
 * A029746  Numbers of the form 2^k or 7*2^k.
 * https://oeis.org/A029746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029746: Numbers of the form 2^k or 7*2^k merged in ascending order. */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t curP   = 1;        /* current 2^k */
    seq_t curSeven = 7;      /* current 7*2^k */
    idx_t i = 0;             /* index of returned element */

    for (;;) {
        seq_t val;
        if (curP <= curSeven) {           /* next smallest is a power of two */
            val = curP;
            curP = mul_small(curP, 2);    /* go to next power of two */
        } else {                          /* next smallest is 7*2^k */
            val = curSeven;
            curSeven = mul_small(curSeven, 2); /* go to next 7*2^k */
        }

        if (i == n)
            return val;
        ++i;
    }
}
