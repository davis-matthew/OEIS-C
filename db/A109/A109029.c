/*
 * A109029  Numbers that have exactly nine prime factors counted with
 *          multiplicity (A046312) whose digit reversal is different and also
 *          has 9 prime factors (with multiplicity).
 * https://oeis.org/A109029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   bigomega_uint64, rev_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A109029: Numbers that have exactly nine prime factors counted
 * with multiplicity (Ω(n)=9) whose decimal digit reversal is different
 * and also has Ω=9.
 * 
 * a(n) returns the nth such number (offset 1).
 * The implementation performs a linear search, testing each integer
 * for the required property.
 */
#include <stdlib.h>
#include <stdint.h>

/* Helper functions provided by the build */
#include "../utils/bigomega_uint64.h"
#include "../utils/rev_uint64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t cur = 1;            /* start searching from 2 upwards */

    for (;;) {
        cur++;                                       /* next candidate */

        /* check Ω(cur) */
        if (bigomega_uint64((uint64_t)cur) != 9)
            continue;

        /* compute reversal */
        seq_t rev = rev_uint64(cur);

        /* reversal must be different */
        if (rev == cur)
            continue;

        /* check Ω(rev) */
        if (bigomega_uint64((uint64_t)rev) != 9)
            continue;

        /* found a valid term */
        count++;
        if (count == n)
            return (seq_t)cur;
    }
}
