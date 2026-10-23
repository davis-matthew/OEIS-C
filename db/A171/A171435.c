/*
 * A171435  Product of odd prime factors < n, with multiplicity.
 * https://oeis.org/A171435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≤ 2 or n is prime or n is a power of two; n if n
 *            is an odd composite; n with all factors of 2 removed, i.e. the
 *            odd part of n, otherwise
 * Range:     u8 n <= 258, u16 n <= 9746, u32 n <= 9746, u64 n <= 9746,
 *            u128 n <= 9746, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n <= 2)
        return (seq_t)1;

    /* Even n: return the odd part (remove all factors of 2) */
    if ((n & 1ULL) == 0) {
        idx_t m = n;
        while ((m & 1ULL) == 0)
            m >>= 1;
        return (seq_t)m;   /* m == 1 for pure powers of 2, giving 1 */
    }

    /* Odd n > 2: prime numbers yield 1, otherwise the number itself */
    return is_prime(n) ? (seq_t)1 : (seq_t)n;
}
