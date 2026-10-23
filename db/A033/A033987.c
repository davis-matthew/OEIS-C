/*
 * A033987  Numbers that are divisible by at least 4 primes (counted with
 *          multiplicity).
 * https://oeis.org/A033987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are divisible by at least 4 primes (counted with multiplicity). */
/*  Implementation: iterate over natural numbers and retain those whose total
 *  number of prime factors (with multiplicity) is ≥ 4, counting from 1. */

#include <stdint.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /*  n is 1‑based index */
    if (n == 0)
        return 0;   /* undefined for offset < 1; harmless */

    idx_t found = 0;
    idx_t candidate = 1;        /* start scanning from 2 */

    while (found < n)
    {
        ++candidate;           /* next integer */

        /* Count total multiplicity of prime factors of candidate.   */
        seq_t omega = count_prime_factors((seq_t)candidate);

        if (omega >= 4)
            ++found;           /* candidate is the next term of the sequence */
    }

    return (seq_t)candidate;    /* candidate holds a(n) */
}
