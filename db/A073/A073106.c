/*
 * A073106  Least k such that there are n primes among the numbers 2*n*x + 1
 *          for 1 <= x <= k.
 * https://oeis.org/A073106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k such that among numbers 1 + 2*n*x (1 ≤ x ≤ k)
 *            there are exactly n primes. (This is the index of the n-th prime
 *            in the arithmetic progression 1 (mod 2n).)
 * Range:     u8 n <= 7, u16 n <= 360, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    idx_t primes_needed = n;   /* number of primes to find */
    idx_t k = 0;               /* current index x */
    idx_t primes_found = 0;    /* primes seen so far */

    while (primes_found < primes_needed) {
        ++k;
        /* Compute 1 + 2*n*k as seq_t (avoid overflow for large n/k). */
        seq_t val = (seq_t)2 * (seq_t)n * (seq_t)k + 1;
        if (isPrimeGeneric(val))
            ++primes_found;
    }
    return (seq_t)k;
}
