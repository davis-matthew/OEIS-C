/*
 * A163612  Primes of form 5207*n + 1.
 * https://oeis.org/A163612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that p ≡ 1 (mod 5207)
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* a(1) is the first prime of the progression 5207·k + 1. */
    if (n == 0)
        return 0;               /* undefined for n < 1, return 0 as a guard */

    idx_t count = 0;            /* number of primes found so far   */
    idx_t k     = 1;            /* index in the arithmetic progression */

    while (count < n) {
        seq_t candidate = (seq_t)5207 * (seq_t)k + (seq_t)1;
        if (isPrimeGeneric(candidate)) {   /* primality test for seq_t */
            ++count;
            if (count == n)
                return candidate;         /* found the n-th term */
        }
        ++k;
    }

    /* Unreachable, but keep the compiler happy. */
    return 0;
}
