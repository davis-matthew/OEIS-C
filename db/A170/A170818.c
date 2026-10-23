/*
 * A170818  a(n) is the product of primes (with multiplicity) of form 4*k+1
 *          that divide n.
 * https://oeis.org/A170818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏{ p^e | p^e divides n , p ≡ 1 (mod 4) } p^e (product of
 *            all prime factors ≡ 1 mod 4, counted with multiplicity)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* OEIS offset 1: a(1)=1.  For n==0 just return 1. */
    if (n < 1) return (seq_t)1;

    seq_t prod = (seq_t)1;
    idx_t m = n;

    /* trial division up to sqrt(m) */
    idx_t i = 2;
    while (i <= m / i) {
        while (m % i == 0) {
            if ((i & 3u) == 1u)          /* i % 4 == 1 */
                prod *= (seq_t)i;
            m /= i;
        }
        if (i == 2) i = 3;
        else i += 2;                   /* skip even numbers */
    }

    /* if a prime factor > sqrt(n) remains */
    if (m > 1 && (m & 3u) == 1u)
        prod *= (seq_t)m;

    return prod;
}
