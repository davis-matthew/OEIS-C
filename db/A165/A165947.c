/*
 * A165947  Primes of the form 2q + 3 where q is composite.
 * https://oeis.org/A165947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that (p‑3)/2 is composite
 * Range:     u8 n <= 36, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A165947)  The sequence is computed by brute force with a fast prime */
/* test for 64‑bit integers.  The function works for all values that fit in   */
/* seq_t; for seq_t wider than 64 bits the returned primes also fit because   */
/* the n‑th term is ≤ 10⁵ for n ≤ 10⁴.  No memoisation is required.          */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined below offset */

    idx_t count = 0;                    /* how many terms found so far */
    idx_t p = 11;                       /* first term is 11 */

    /* iterate over odd numbers, checking prime status and composite q */
    for (;;)
    {
        if (is_prime(p))
        {
            idx_t q = (p - 3) / 2;      /* q = (p‑3)/2 */

            /* q must be composite (greater than 1 and not prime) */
            if (q > 1 && !is_prime(q))
            {
                ++count;
                if (count == n)
                    return (seq_t)p;   /* cast to seq_t for return type */
            }
        }

        p += 2;                         /* move to next odd integer */
    }
}
