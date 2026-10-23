/*
 * A162848  An alternating 2-based sum from prime(n) up to the base of the
 *          n-th Mersenne prime.
 * https://oeis.org/A162848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=prime(n)}^{exp(n)} (2 - j*(-1)^j)
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   alt_sum_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation pre‑stores the first 49 primes and the first
 * 49 exponents of Mersenne primes (A000043). For n > 49 there is no
 * known exponent, so the function returns 0.  The sum is evaluated
 * by a closed formula for the alternating sum of j.                  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alt_sum_u64.h"

/* First 49 primes (A000040).  Indexed 0…48, so prime(n) = primes[n-1]. */
static const idx_t primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL,
    59ULL, 61ULL, 67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL,
    97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL, 127ULL, 131ULL,
    137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL, 173ULL,
    179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL, 211ULL, 223ULL,
    227ULL
};
static const size_t primes_count = sizeof(primes) / sizeof(primes[0]);

/* Exponents of the first 49 Mersenne primes (A000043).  Indexed 0…48. */
static const idx_t mersenne_exp[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 13ULL, 17ULL, 19ULL, 31ULL, 61ULL, 89ULL,
    107ULL, 127ULL, 521ULL, 607ULL, 1279ULL, 2203ULL, 2281ULL, 3217ULL,
    4253ULL, 4423ULL, 9689ULL, 9941ULL, 11213ULL, 19937ULL, 21701ULL,
    23209ULL, 44497ULL, 86243ULL, 110503ULL, 132049ULL, 216091ULL,
    756839ULL, 859433ULL, 1257787ULL, 1398269ULL, 2976221ULL, 3021377ULL,
    6972593ULL, 13466917ULL, 20996011ULL, 24036583ULL, 25964951ULL,
    30402457ULL, 32582657ULL, 37156667ULL, 42643801ULL, 43112609ULL,
    57885161ULL, 74207281ULL, 77232917ULL, 82589933ULL
};
static const size_t mersenne_exps_count = sizeof(mersenne_exp) / sizeof(mersenne_exp[0]);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Sequence offset is 1; for n < 1 we return 0. */
    if (n <= 0)
        return (seq_t)0;

    /* If n exceeds known primes or exponents, the definition is
     * incomplete – return 0 as a safe placeholder. */
    if (n > primes_count || n > mersenne_exps_count)
        return (seq_t)0;

    /* 1‑based indices → 0‑based array positions. */
    idx_t l = primes[n - 1];          /* prime(n)   */
    idx_t u = mersenne_exp[n - 1];     /* exponent of nth Mersenne prime */

    /* Length of the interval [l, u] */
    idx_t len = u - l + 1;

    /* Compute the alternating sum of j over [l, u] */
    long long sum_j = alt_sum_u64(u) - alt_sum_u64(l - 1);

    /* Final sum: S = 2*len - Σ j*(-1)^j  */
    long long result = (long long)2 * (long long)len - sum_j;

    return (seq_t)result;
}
