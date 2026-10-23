/*
 * A135585  a(n) = Sum_{i=1..n} (floor(S2(i)/3) mod 2), where S2(i) =
 *          A000120(i).
 * https://oeis.org/A135585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Sum_{i=1..n} (floor(S2(i)/3) mod 2)
 * Range:     u8 n <= 345, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes a(n) for OEIS A135585.
   n is of type idx_t (uint64_t). The sum is accumulated in seq_t. */
seq_t a(idx_t n)
{
    /* n = 0 corresponds to the first term a(0) = 0 */
    if (n == 0ULL)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    for (idx_t i = 1ULL; i <= n; ++i)
    {
        /* number of 1-bits in i */
        unsigned int pop = __builtin_popcountll((unsigned long long)i);

        /* (floor(pop/3)) mod 2 */
        unsigned int term = (pop / 3) & 1U;

        sum = sum + (seq_t)term;
    }
    return sum;
}
