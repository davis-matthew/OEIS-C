/*
 * A140850  Primes of the form 210k + 47.
 * https://oeis.org/A140850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p % 210 == 47 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Return the nth prime of the form 210k + 47.
   a(1) = 47, a(2) = 257, ...
   The function works for any n >= 1, in any call order. */
seq_t a(idx_t n)
{
    if (n < 1) /* undefined for n < 1 */
        return 0;

    idx_t count = 0;   /* how many primes of the form 210k+47 have been seen */
    idx_t k = 0;       /* current multiplier for the 210-step sequence */

    while (true) {
        /* Candidate value: 47 + 210*k.  Cast k to seq_t to avoid overflowing idx_t. */
        seq_t candidate = 47 + (seq_t)210 * (seq_t)k;

        if (is_prime_small(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }

        ++k;
    }
}
