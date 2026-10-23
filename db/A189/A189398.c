/*
 * A189398  a(n) = 2^d(1) * 3^d(2) * ... * prime(k)^d(k), where
 *          d(1)d(2)...d(k) is the decimal representation of n.
 * https://oeis.org/A189398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = 2^d(1) * 3^d(2) * ... * prime(k)^d(k) where d(i) is the
 *            i‑th decimal digit of n (most significant first), and prime(i)
 *            is the i‑th prime (2,3,5,7,11,13,…)
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 178, u64 n <= 4998,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) is outside the offset; just return 1 for safety */
    if (n == 0)
        return (seq_t)1;

    /* first 20 primes: enough for any uint64_t n (max 20 decimal digits) */
    static const uint64_t primes[20] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL,
        59ULL, 61ULL, 67ULL, 71ULL
    };

    idx_t temp   = n;
    idx_t divisor = 1;

    /* find highest power of 10 ≤ temp */
    while (divisor <= temp / 10)
        divisor *= 10;

    seq_t result = 1;
    idx_t digit_index = 0;   /* position of current digit → prime index */

    while (temp > 0) {
        idx_t digit = temp / divisor;          /* most significant remaining digit */
        int exp = (int)digit;                  /* exponent is small (0..9) */

        if (exp > 0) {
            seq_t prime = (seq_t)primes[digit_index];
            seq_t powed  = big_pow(prime, (seq_t)exp);
            result *= powed;
        }

        temp   %= divisor;
        divisor /= 10;
        digit_index++;
    }

    return result;
}
