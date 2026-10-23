/*
 * A137441  Partial sums of partial sums of PrimePi(k).
 * https://oeis.org/A137441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} (n+1-i) * pi(i) * where pi(i) is the number
 *            of primes ≤ i
 * Range:     u8 n <= 13, u16 n <= 107, u32 n <= 5573, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is undefined in OEIS; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Allocate a composite marker array of size n+1. */
    uint8_t *composite = (uint8_t *)calloc((size_t)(n + 1), sizeof(uint8_t));
    if (!composite) return (seq_t)0;      /* allocation failure */

    /* Simple Eratosthenes sieve up to n. */
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (!composite[p])
        {
            for (idx_t m = p * p; m <= n; m += p)
                composite[m] = 1;
        }
    }

    /* Compute the required sum.  We maintain pi(i) incrementally. */
    seq_t pi = 0;
    seq_t sum = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        if (i >= 2 && !composite[i])
            ++pi;

        seq_t term = ((seq_t)(n + 1 - i)) * pi;
        sum += term;
    }

    free(composite);
    return sum;
}
