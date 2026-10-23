/*
 * A141866  Primes of the form 2*3*5*7*k+89, k >= 0.
 * https://oeis.org/A141866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 103, u32 n <= 104, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime of the form 210*k + 89 (2*3*5*7*k + 89). */
/* The function iterates k starting from 0, checks each number for
   primality using is_prime_big, and returns the n-th prime found. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* Offset is 1, though n==0 is unused */

    idx_t found = 0;
    idx_t k = 0;

    while (true)
    {
        seq_t m = (seq_t)210 * k + (seq_t)89;
        if (is_prime_big(m))
        {
            ++found;
            if (found == n)
                return m;
        }
        ++k;
    }
}
