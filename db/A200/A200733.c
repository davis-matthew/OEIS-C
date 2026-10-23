/*
 * A200733  Primes of the form 4*n^3-7.
 * https://oeis.org/A200733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 4*m^3-7 (OEIS A200733)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   cube, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_seq.h"
#include "../utils/cube.h"

/* Return the n-th prime of the form 4*m^3-7.
   Indexing starts at n = 1. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m     = 1;
    seq_t candidate;

    while (1)
    {
        /* Compute 4*m^3 - 7 as a seq_t expression. */
        candidate = ((seq_t)4 * cube((seq_t)m)) - (seq_t)7;

        if (is_prime_seq(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        ++m;
    }
}
