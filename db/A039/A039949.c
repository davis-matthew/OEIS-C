/*
 * A039949  Primes of the form 30n - 13.
 * https://oeis.org/A039949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 17 (mod 30)
 * Range:     u8 n <= 7, u16 n <= 725, u32 n <= 726, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Computes the nth prime of the form 30k-13.
   The sequence starts at a(1) = 17. */
seq_t a(idx_t n)
{
    seq_t cand = 17;      /* first number of the form 30k-13 */
    idx_t  count = 0;

    while (true)
    {
        /* test primality; works for arbitrary‑size seq_t */
        if (is_prime_big(cand))
        {
            ++count;
            if (count == n)
                return cand;
        }
        cand += (seq_t)30;   /* next candidate of the form 30k-13 */
    }
}
