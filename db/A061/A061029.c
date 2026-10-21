/*
 * A061029  2^(n-1)*A000668(n).
 * https://oeis.org/A061029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) * (2^p_n – 1), where p_n is the n‑th exponent of
 *            a Mersenne prime
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Known exponents of Mersenne primes (p_n).  Extend as needed. */
    static const idx_t mersenne_exps[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 13ULL, 17ULL, 19ULL, 31ULL,
        61ULL, 89ULL, 107ULL, 127ULL, 521ULL, 607ULL, 1279ULL,
        2203ULL, 2281ULL, 3217ULL, 4253ULL, 4423ULL, 9689ULL,
        9941ULL, 11213ULL, 19937ULL, 21701ULL, 23209ULL,
        44497ULL, 86243ULL, 110503ULL, 132049ULL, 216091ULL,
        756839ULL, 859433ULL, 1257787ULL, 1398269ULL,
        2976221ULL, 3021377ULL
    };
    const size_t M = sizeof(mersenne_exps) / sizeof(mersenne_exps[0]);

    if (n == 0) return 0;              /* undefined; guard against n < 1       */
    if ((idx_t)(n - 1) >= M) return 0;  /* undefined beyond known exponents   */

    idx_t exp = mersenne_exps[n - 1];   /* p_n */
    seq_t base = (seq_t)2;

    /* Compute 2^(n-1) and 2^p_n */
    seq_t pow_small = big_pow(base, (seq_t)(n - 1));
    seq_t pow_big   = big_pow(base, exp);

    /* a(n) = 2^(n-1) * (2^p_n – 1) */
    seq_t result = seq_mul(pow_small, pow_big - (seq_t)1);
    return result;
}
