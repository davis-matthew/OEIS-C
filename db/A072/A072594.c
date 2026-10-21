/*
 * A072594  In prime factorization of n replace multiplication with bitwise
 *          logical 'xor'.
 * https://oeis.org/A072594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A072594 */
#include <stdint.h>
#include <stdbool.h>

/* A(n) is the XOR of the primes appearing an odd number of times in
   the prime factorization of n.  For n=1, A(1)=1.  This function
   factorises n by trial division; it works for any idx_t (uint64_t)
   and any seq_t that supports the C-type arithmetic operators. */

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t res = (seq_t)0;

    /* factor 2 */
    int exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        exp++;
    }
    if (exp & 1)
        res ^= (seq_t)2;

    /* factor odd primes */
    for (uint64_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            exp = 0;
            while (m % i == 0) {
                m /= i;
                exp++;
            }
            if (exp & 1)
                res ^= (seq_t)i;
        }
    }

    /* remaining prime factor */
    if (m > 1)
        res ^= (seq_t)m;

    return res;
}
