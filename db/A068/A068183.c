/*
 * A068183  Largest number without decimal digits equal to 1 whose product of
 *          digits gives n!.
 * https://oeis.org/A068183
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, fini, nonn, full
 * Formula:   For n ≥ 2, a(n) is the largest integer whose decimal digits *
 *            (each in {2,…,9}, none equal to 1) multiply to n!. Since n!
 *            contains * only the primes 2, 3, 5, 7 for n ≤ 10, the optimal
 *            representation * is simply the multiset of those prime digits
 *            sorted descending. * For n > 10 no such factorisati...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   prime_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prime_exp.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    if (n > 10)
        return (seq_t)0;   /* no solution for n > 10 */

    /* Exponents of the prime factors 2,3,5,7 in n! */
    idx_t e2   = prime_exp(n, 2);
    idx_t e3   = prime_exp(n, 3);
    idx_t e5   = prime_exp(n, 5);
    idx_t e7   = prime_exp(n, 7);

    seq_t res = 0;

    /* Append digits in descending order to maximise the integer value */
    for (idx_t i = 0; i < e7; ++i) res = res * 10 + 7;
    for (idx_t i = 0; i < e5; ++i) res = res * 10 + 5;
    for (idx_t i = 0; i < e3; ++i) res = res * 10 + 3;
    for (idx_t i = 0; i < e2; ++i) res = res * 10 + 2;

    return res;
}
