/*
 * A153144  Numbers n such that 2*n+19 is not a prime.
 * https://oeis.org/A153144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer m such that 2*m+19 is not prime. The
 *            sequence starts at a(1)=1
 * Range:     u8 n <= 78, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so any n < 1 is undefined.  Return 0
       merely to satisfy a non‑zero value for such calls. */
    if (n == 0) return (seq_t)0;

    idx_t count = 0;
    for (idx_t m = 0; ; ++m) {
        seq_t val = (seq_t)m * (seq_t)2 + (seq_t)19;
        if (!isPrimeGeneric(val)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
