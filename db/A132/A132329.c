/*
 * A132329  Numbers that cannot be expressed as the difference of the squares
 *          of primes.
 * https://oeis.org/A132329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime_big, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that cannot be expressed as the difference of squares of primes.
   The function checks each integer sequentially, testing whether it can
   be written as p²−q² for primes p>q. It counts those that cannot and
   returns the nth such number. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many non‑expressible numbers we have found */
    seq_t cand   = 1;         /* current candidate to test */

    for (;; ++cand)
    {
        bool expressible = false;

        /* factor cand into a*b with a ≤ b */
        seq_t limit = sqrt_floor(cand);
        for (seq_t a = 1; a <= limit; ++a)
        {
            if (cand % a != 0)
                continue;

            seq_t b = cand / a;
            if (a >= b)               /* need a<b to have q>0 */
                break;
            if ( (a & 1) != (b & 1) ) /* a and b must have same parity */
                continue;

            seq_t p = (a + b) >> 1;   /* p = (a+b)/2  */
            seq_t q = (b - a) >> 1;   /* q = (b-a)/2  */
            if (is_prime_big(p) && is_prime_big(q))
            {
                expressible = true;
                break;
            }
        }

        if (!expressible)
        {
            ++count;
            if (count == n)
                return cand;
        }
    }
}
