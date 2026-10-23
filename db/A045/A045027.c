/*
 * A045027  Numbers whose base-4 representation contains exactly two 0's and
 *          four 1's.
 * https://oeis.org/A045027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1420, u32 n <= 4300, u64 n <= 4300,
 *            u128 n <= 4300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045027: numbers whose base‑4 representation contains exactly two 0's and four 1's.
   Computation by brute‑force enumeration. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t count = 0;

    while (count < n) {
        val++;                    /* next integer to test */
        seq_t x = val;
        unsigned cnt0 = 0, cnt1 = 0;

        while (x > 0) {
            int d = (int)(x % 4); /* last base‑4 digit */
            if (d == 0) cnt0++;
            else if (d == 1) cnt1++;
            if (cnt0 > 2 || cnt1 > 4) break; /* early exit */
            x = x / 4;
        }
        if (cnt0 == 2 && cnt1 == 4 && x == 0)
            count++;             /* found a qualifying number */
    }
    return val;                   /* val is the nth term */
}
