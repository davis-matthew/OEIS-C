/*
 * A202112  Numbers k such that 90*k + 79 is prime.
 * https://oeis.org/A202112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers k such that 90*k + 79 is prime. a(n) returns the nth
 *            such k
 * Range:     u8 n <= 1, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Count how many k have met the condition. */
    idx_t count = 0;

    for (idx_t k = 0; ; ++k)
    {
        /* Compute the candidate number 90*k + 79 entirely in seq_t to avoid overflow. */
        seq_t cand = (seq_t)90 * (seq_t)k + (seq_t)79;

        if (isPrimeGeneric(cand))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
