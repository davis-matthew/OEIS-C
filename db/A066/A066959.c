/*
 * A066959  a(n) = bigomega(n^n) where bigomega(x) is the number of prime
 *          factors in x (counted with multiplicity).
 * https://oeis.org/A066959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Ω(n) (Ω(n) = number of prime factors of n counted
 *            with multiplicity)
 * Range:     u8 n <= 63, u16 n <= 6143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* behavior below the offset is irrelevant */

    return (seq_t)n * bigomega(n);
}
