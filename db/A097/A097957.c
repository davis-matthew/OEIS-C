/*
 * A097957  Primes p such that p divides 5^((p-1)/2) + 4^((p-1)/2).
 * https://oeis.org/A097957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097957: primes p such that p divides 5^((p-1)/2)+4^((p-1)/2).
   Equivalent to odd primes p ≡ 2 or 3 (mod 5). This implementation
   searches for the nth such prime (offset 1) using the provided
   is_prime() routine. */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* offset 1; return 0 for safety */

    idx_t found = 0;           /* count of qualifying primes found so far */
    idx_t p = 3;               /* start checking from the first odd prime */

    while (1) {
        if (is_prime(p)) {
            if ((p % 5 == 2) || (p % 5 == 3)) {
                ++found;
                if (found == n)
                    return (seq_t)p;
            }
        }
        /* move to the next odd candidate */
        if (p == 3)
            p = 5;
        else
            p += 2;
    }
}
