/*
 * A031976  Remove digits 0, 1, 4, 9 from n.
 * https://oeis.org/A031976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   strip_forbidden
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031976  
   a(n) = the n-th positive integer obtained by deleting all decimal
   digits 0, 1, 4, 9 from successive natural numbers starting at 1.  */
#include <stdint.h>
#include "../utils/strip_forbidden.h"

seq_t a(idx_t n)
{
    idx_t candidate = 1;
    idx_t count = 0;
    for (;;) {
        seq_t val = strip_forbidden((uint64_t)candidate);
        if (val != 0) {
            ++count;
            if (count == n) {
                return val;
            }
        }
        ++candidate;
    }
}
