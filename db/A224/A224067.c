/*
 * A224067  Number of cycles the reduced complex Collatz function using 1 +
 *          (2n-1)i takes to hit 1 + i, or 0 if this never happens.
 * https://oeis.org/A224067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rc_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Collatz‑cycle length for the reduced complex Collatz function.
 * a(n) = number of terms until the sequence starting at 1 + (2n–1)i
 * reaches 1 + i, or 0 if 1 + i is never reached.
 */

#include <stdint.h>
#include "../utils/rc_next.h"

seq_t a(idx_t n)
{
    /* initial odd coordinates for the reduced Collatz function */
    long long tort_x = 1;
    long long tort_y = 2LL * (long long)n - 1LL;   /* start point */

    /* hare starts at the same position but will advance twice per loop */
    long long hare_x = tort_x, hare_y = tort_y;

    /* length of the visited sequence so far (including the start point) */
    idx_t len = 1;
    if (tort_x == 1 && tort_y == 1)
        return (seq_t)len;          /* trivial cycle 1+i → 1+i */

    while (1) {
        /* advance tortoise by one step */
        rc_next(&tort_x, &tort_y);
        ++len;
        if (tort_x == 1 && tort_y == 1)
            return (seq_t)len;      /* first time we hit 1+i */

        /* advance hare by two steps */
        rc_next(&hare_x, &hare_y);
        rc_next(&hare_x, &hare_y);
        if (hare_x == tort_x && hare_y == tort_y)
            return (seq_t)0;          /* cycle detected, 1+i not reached */
    }
}
