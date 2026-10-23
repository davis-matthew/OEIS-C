/*
 * A147603  Primes congruent to (1,1,2) mod 3, see comment.
 * https://oeis.org/A147603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the pattern a(3k+1) ≡ 1 (mod 3), a(3k+2) ≡ 1
 *            (mod 3), a(3k+3) ≡ 2 (mod 3). The code produces the sequence by
 *            iterating over primes in increasing order and accepting those
 *            that match the cyclic congruence
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
/* Primality helper supplied by the project. */
#include "../utils/is_prime.h"

bool is_prime(idx_t x);

seq_t a(idx_t n)
{
    if (n < 1U) return 0;                 /* undefined for n < 1 */

    idx_t        found = 0;               /* number of terms found so far */
    unsigned     state = 0;               /* 0->expect 1 mod 3, 1->expect 1 mod 3, 2->expect 2 mod 3 */
    seq_t        result = 0;
    idx_t        p = 5;                   /* start scanning from 5 (odd numbers only) */

    while (found < n)
    {
        if (is_prime(p))
        {
            if (p % 3U == 1U)              /* prime ≡ 1 (mod 3) */
            {
                if (state <= 1U)           /* positions 3k+1 or 3k+2 */
                {
                    ++found;
                    if (found == n)
                    {
                        result = (seq_t)p;
                        break;
                    }
                    state = (state + 1U) % 3U;
                }
            }
            else /* p % 3U == 2U */        /* prime ≡ 2 (mod 3) */
            {
                if (state == 2U)           /* position 3k+3 */
                {
                    ++found;
                    if (found == n)
                    {
                        result = (seq_t)p;
                        break;
                    }
                    state = 0U;
                }
            }
        }
        p += 2;                           /* only odd candidates needed */
    }

    return result;
}
