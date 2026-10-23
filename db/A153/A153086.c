/*
 * A153086  Numbers n such that 4*n+7 is not prime.
 * https://oeis.org/A153086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n-th integer such that 4*n+7 is composite. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1, so n>=1 in normal use */
        return 0;

    idx_t count = 0;
    idx_t i = 1;

    for (;; ++i) {
        idx_t val = 4 * i + 7;          /* candidate 4*i+7, stays in idx_t */
        if (!isPrimeGeneric((seq_t)val)) {   /* test primality */
            ++count;
            if (count == n)
                return (seq_t)i;           /* return the integer i itself */
        }
    }
}
