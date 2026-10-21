/*
 * A086545  Smallest k such that n*k+1 is a prime and (n-1)/k is noncomposite,
 *          or 0 if no such number exists.
 * https://oeis.org/A086545
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that n*k+1 is prime and (n-1)/k is
 *            non‑composite (i.e. 1 or prime). Return 0 if no such k exists.
 *            The function is called for n ≥ 2. It enumerates all divisors k
 *            of n‑1 in increasing order and checks the two conditions using
 *            the provided generic primality tester
 * Range:     u8 n <= 23, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at 2 */

    idx_t rem = n - 1;            /* n-1 must be divisible by k */

    /* Enumerate all divisors k of n-1 in ascending order. */
    for (idx_t k = 1; k <= rem; ++k) {
        if (rem % k != 0) continue;      /* k is not a divisor */

        idx_t t = rem / k;                /* (n-1)/k */
        /* Check whether t is 1 or a prime. */
        if (!(t == 1 || isPrimeGeneric((seq_t)t))) continue;

        /* Check whether n*k + 1 is prime.  Compute the product in idx_t to
           avoid overflow, then cast to seq_t for the primality test. */
        idx_t pk64 = n * k + 1;
        seq_t pk = (seq_t)pk64;
        if (!isPrimeGeneric(pk)) continue;

        /* k satisfies both conditions; return the smallest such k. */
        return (seq_t)k;
    }

    /* No valid k found. */
    return 0;
}
