/*
 * A144790  Consider the runs of 1's in the binary representation of n, each
 *          of these runs being on the edge of the binary representation n
 *          and/or being bounded by 0's. a(n) = the length of the shortest
 *          such run of 1's in binary n.
 * https://oeis.org/A144790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9421, u16 n <= 9421, u32 n <= 9421, u64 n <= 9421,
 *            u128 n <= 9421, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A144790: For n ≥ 1, a(n) is the length of the
   shortest run of 1s in the binary representation of n.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* outside offset – undefined, return 0 */

    /* determine an upper bound for the answer: number of bits of n */
    idx_t tmp = n;
    seq_t min_run = 0;
    while (tmp) {                /* count bits of n */
        ++min_run;
        tmp >>= 1;
    }

    /* scan bits from LSB to MSB finding runs of 1s */
    seq_t cur = 0;
    tmp = n;
    while (tmp) {
        if (tmp & 1) {
            ++cur;
        } else {
            if (cur) {
                if (cur < min_run) min_run = cur;
                cur = 0;
            }
        }
        tmp >>= 1;
    }
    if (cur)               /* trailing run of 1s when n ends in 1 */
        if (cur < min_run) min_run = cur;

    return min_run;
}
