/*
 * A153801  Index of Mersenne number A000225 that is also Mersenne prime
 *          A000668, minus n-th prime: a(n) = A000043(n) - A000040(n).
 * https://oeis.org/A153801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000043(n) - A000040(n)
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"

static const uint64_t mersenne_exponents[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 13ULL, 17ULL, 19ULL, 31ULL, 61ULL, 89ULL,
    107ULL, 127ULL, 521ULL, 607ULL, 1279ULL, 2203ULL, 2281ULL, 3217ULL,
    4253ULL, 4423ULL, 9689ULL, 9941ULL, 11213ULL, 19937ULL,
    21701ULL, 23209ULL, 44497ULL, 86243ULL, 110503ULL, 132049ULL,
    216091ULL, 756839ULL, 859433ULL, 1257787ULL, 1398269ULL, 2976221ULL,
    3021377ULL, 6972593ULL, 13466917ULL, 20996011ULL, 24036583ULL,
    25964951ULL, 30402457ULL, 32582657ULL, 37156667ULL, 42643801ULL,
    43112609ULL, 57885161ULL, 74207281ULL, 77232917ULL, 82589933ULL
};
static const size_t known_count =
    sizeof(mersenne_exponents) / sizeof(mersenne_exponents[0]);

seq_t a(idx_t n)
{
    if (n < 1 || n > known_count) {
        return (seq_t)0;
    }
    seq_t exp   = (seq_t)mersenne_exponents[n - 1];
    seq_t prime = (seq_t)get_prime(n);
    return exp - prime;
}
