/*
 * A084344  A077982(n) >= 5.
 * https://oeis.org/A084344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   floor_cube_root, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th odd integer greater than 1 that is NOT of the form
 * 1, prime p, p+2 (twin with 2), product of two primes p*q, or a prime cube p^3.
 * The offset is 1, so a(1) = 117, a(2) = 135, ...
 * (OEIS A084344)
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0, return 0 */

    idx_t target = (idx_t)n;
    seq_t found = 0;
    seq_t candidate = 0;

    /* iterate over odd integers starting from 3 */
    for (seq_t x = 3; ; x += 2)
    {
        /* 1 is excluded, primes are excluded */
        if (isPrimeGeneric(x)) continue;

        /* exclude numbers of the form p + 2 (i.e., x-2 is prime) */
        if (isPrimeGeneric(x - 2)) continue;

        /* exclude prime cubes */
        seq_t r = floor_cube_root(x);
        if (r * r * r == x) continue;

        /* exclude product of two primes (including squares of a prime) */
        bool prod_two_prime = false;
        for (seq_t p = 3; p * p <= x; p += 2)
        {
            if (!isPrimeGeneric(p)) continue;
            if (x % p == 0)
            {
                seq_t q = x / p;
                if (isPrimeGeneric(q))
                {
                    prod_two_prime = true;
                    break;
                }
            }
        }
        if (prod_two_prime) continue;

        /* x passes all tests and is a desired term */
        found++;
        if (found == target)
        {
            candidate = x;
            break;
        }
    }

    return candidate;
}
