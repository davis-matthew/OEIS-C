/*
 * A200849  Primes of the form 3n^3-5.
 * https://oeis.org/A200849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime of the form 3*k³-5 by brute‑force search.
   a(1) = 19, a(2) = 643, a(3) = 1531, … as listed in the OEIS entry
   for A200849. */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    idx_t   found = 0;
    seq_t   k     = 2;            /* start with k = 2 → 3*2³-5 = 19 */

    while (1) {
        seq_t val = 3 * k * k * k - 5;   /*   3*k³ – 5   */

        if (is_prime_seq(val))
        {
            ++found;
            if (found == n)
                return val;
        }
        ++k;
    }
}
