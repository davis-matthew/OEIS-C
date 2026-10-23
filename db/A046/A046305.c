/*
 * A046305  Numbers that are divisible by at least 6 primes (counted with
 *          multiplicity).
 * https://oeis.org/A046305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer whose total number of * prime
 *            factors (counted with multiplicity) is at least 6
 * Range:     u8 n <= 9, u16 n <= 7274, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    idx_t count = 0;              /* numbers already found */
    idx_t val   = 0;              /* current candidate */

    while (count < n)
    {
        ++val;
        if (bigomega(val) >= 6)
            ++count;
    }
    return (seq_t)val;
}
