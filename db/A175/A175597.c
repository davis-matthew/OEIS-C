/*
 * A175597  Minimal run length in binary representation of n.
 * https://oeis.org/A175597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Minimal run length in binary representation of n
    a(1) = 1
    a(2) = 1
    a(3) = 2
    a(4) = 1
    a(5) = 1
    a(6) = 1
    ...
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* n >= 1 (OEIS offset starts at 1) */
    idx_t m = n;                                   /* copy of the index */
    /* maximal possible run length is the number of bits in n */
    seq_t min_run = (seq_t)(bitlen_seq_t(n) + 1);
    seq_t run = 1;                                 /* current run length */
    uint8_t prev_bit = (n & 1U) ? 1 : 0;             /* first bit (LSB) */
    m >>= 1;                                       /* skip first bit */

    while (m != 0) {
        uint8_t bit = (m & 1U) ? 1 : 0;
        if (bit == prev_bit) {
            run++;                                 /* extend current run */
        } else {
            if (run < min_run)
                min_run = run;                    /* finish previous run */
            prev_bit = bit;
            run = 1;                               /* start new run */
        }
        m >>= 1;
    }

    /* account for the last run */
    if (run < min_run)
        min_run = run;

    return min_run;
}
