/*
 * A124589  Primes p such that q-p <= 4, where q is the next prime after p.
 * https://oeis.org/A124589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that the next prime q after p
 *            satisfies q - p <= 4 Computation: simple on-the-fly prime search
 *            using the provided is_prime helper. Complexity: roughly O(n *
 *            average prime gap) which is well below 2 s for n≈10 000
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th element of the sequence described above (1-indexed). */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many qualifying primes found */
    idx_t p = 2;              /* current prime */

    while (1) {
        /* find the next prime q after p */
        idx_t q = p + 1;
        while (!is_prime(q))
            ++q;

        /* if the gap is at most 4, count this prime */
        if (q - p <= 4) {
            ++count;
            if (count == n)
                return (seq_t)p;   /* found the n-th qualifying prime */
        }

        /* move to the next prime and repeat */
        p = q;
    }
}
