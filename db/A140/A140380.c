/*
 * A140380  Composites of the form 14k + 3.
 * https://oeis.org/A140380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth composite of the form 14k+3 by iterating over integers
   of that form and testing for compositeness.  The sequence starts
   at index 1. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    idx_t count = 0;
    idx_t k = 0;

    while (true) {
        seq_t val = (seq_t)14 * (seq_t)k + (seq_t)3;

        /* Skip 2 and 3: 14*0+3 = 3 and 14*1+3 = 17 . */
        if (val > 1) {
            if (!is_prime_big(val)) {      /* composite */
                count++;
                if (count == n)
                    return val;
            }
        }
        k++;
    }
}
