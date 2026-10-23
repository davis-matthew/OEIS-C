/*
 * A133050  Cubes of Mersenne primes.
 * https://oeis.org/A133050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^p_n - 1)^3, where p_n is the n-th Mersenne prime
 *            exponent
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static list of known Mersenne prime exponents
   and computes the cube of (2^p - 1) with arbitrary precision arithmetic. */

#include <stdint.h>
#include "../utils/big_pow.h"      /* Provides big_pow(seq_t base, seq_t exp) */
#include "../utils/seq_pow.h"     /* Provides seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Known Mersenne prime exponents (first 51 known terms) */
    static const uint32_t exps[] = {
          2,   3,   5,   7,   13,  17,  19,  31,  61,  89,
         107, 127, 521, 607, 1279, 2203, 2281, 3217, 4253, 4423,
         9689, 9941,11213,19937,21701,23209,44497,86243,110503,132049,
        216091,756839,859433,1257787,1398269,2976221,3021377,6972593,13466917,
        20996011,24036583,25964951,30402457,32582657,37156667,42643801,43112609,
        57885161,74207281,77232917,82589931
    };
    static const size_t known_count = sizeof(exps)/sizeof(exps[0]);

    if (n == 0 || n > known_count)
        return (seq_t)0;          /* Undefined for indices beyond known Mersenne primes */

    /* Retrieve the exponent for the n-th Mersenne prime */
    seq_t exp = (seq_t)exps[n - 1];

    /* Compute 2^exp - 1 */
    seq_t mersenne = big_pow((seq_t)2, exp);
    mersenne -= (seq_t)1;          /* (2^exp) - 1 */

    /* Return the cube of the Mersenne number */
    return seq_pow(mersenne, 3);
}
