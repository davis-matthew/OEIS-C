/*
 * A163510  Irregular table read by rows: Write n in binary. For each 1, the
 *          m-th term of row n is the number of 0's between the m-th 1,
 *          reading right to left, and the (m-1)th 1 (or the right side of the
 *          number if m-1 = 0).
 * https://oeis.org/A163510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9543, u16 n <= 9543, u32 n <= 9543, u64 n <= 9543,
 *            u128 n <= 9543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A163510 - Irregular table read by rows
   a(n) is the nth term of the table described in the OEIS entry.
   The table is constructed as follows:
     For each integer r >= 1:
       - Write r in binary.
       - For each '1' from right to left, produce a term equal to the 
         number of consecutive '0' bits between this '1' and the previous
         '1' (or the right edge if it is the rightmost '1').
   The sequence a(n) enumerates all these terms row by row.
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Variable used as a running counter of produced terms. */
    idx_t pos = 1;          /* 1‑based position in the global sequence */

    for (idx_t row = 1; ; ++row) {
        /* Iterate over bits of 'row' from least‑significant to most. */
        seq_t zero_count = 0;          /* zeros seen since last '1' */
        uint64_t bits = (uint64_t)row; /* row fits in 64 bits; idx_t may be larger */

        while (bits) {
            if (bits & 1ULL) {                /* current bit is '1' */
                if (pos == n)                 /* found the requested term */
                    return zero_count;
                pos++;
                zero_count = 0;                /* reset for next segment */
            } else {
                zero_count++;                  /* count a trailing '0' */
            }
            bits >>= 1;                       /* move to next bit */
        }
    }

    /* The function always returns inside the loop, so this point is
       unreachable, but a return statement keeps the compiler happy. */
    return 0;
}
