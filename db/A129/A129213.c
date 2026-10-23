/*
 * A129213  Numbers m such that 7!*m-/+1 are twin primes.
 * https://oeis.org/A129213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129213: a(n) = m such that 7!*m ± 1 are twin primes.
   The function scans m from 1 upward, testing 5040*m-1 and 5040*m+1
   for primality using isPrimeGeneric(). It stops when the nth such m
   is found. */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    seq_t m = 1;
    idx_t count = 0;

    while (true) {
        seq_t v  = (seq_t)5040 * (seq_t)m;
        seq_t a1 = v - 1;
        seq_t a2 = v + 1;

        if (isPrimeGeneric(a1) && isPrimeGeneric(a2)) {
            count++;
            if (count == n) return m;
        }
        m++;
    }
}
