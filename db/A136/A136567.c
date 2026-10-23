/*
 * A136567  a(n) is the number of exponents occurring only once each in the
 *          prime factorization of n.
 * https://oeis.org/A136567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): counts the exponents in the prime factorization of n
   that occur exactly once.  The function factors n by trial division
   and then checks which exponents are unique. */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool */
#include <stddef.h>   /* size_t */

/* a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* a(1) has no prime factors */
    if (n <= 1)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;
    int exps[64];
    size_t cnt = 0;

    /* factorize by trial division */
    for (uint64_t p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            exps[cnt++] = e;
        }
    }

    /* any remaining prime factor contributes exponent 1 */
    if (m > 1) {
        exps[cnt++] = 1;
    }

    /* count exponents that are unique */
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        bool unique = true;
        for (size_t j = 0; j < cnt; ++j) {
            if (i != j && exps[i] == exps[j]) {
                unique = false;
                break;
            }
        }
        if (unique)
            ++result;
    }

    return result;
}
