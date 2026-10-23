/*
 * A166398  Multiples of 17 whose reversal - 1 is also a multiple of 17.
 * https://oeis.org/A166398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 230, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev_mod17
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166398: Multiples of 17 whose reversal - 1 is also a multiple of 17.
   The n-th term is the n-th positive integer x such that
       x ≡ 0 (mod 17)  and  rev(x) ≡ 1 (mod 17),
   where rev(x) is the decimal reversal of x. */

#include <stdint.h>
#include "../utils/rev_mod17.h"

/* Return the n-th term of sequence A166398 (n ≥ 1). */
seq_t a(idx_t n)
{
    seq_t count = 0, val = 0;
    for (idx_t i = 1; ; ++i) {
        val = (seq_t)17 * (seq_t)i;           /* current multiple of 17 */
        if (rev_mod17(val) == 1) {            /* rev(val)-1 is divisible by 17 */
            ++count;
            if (count == n)
                return val;
        }
    }
}
