/*
 * A173105  The 15 supersingular primes written in octal.
 * https://oeis.org/A173105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = A007094(A002267(n))
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007094: write the integer in octal and interpret that string as decimal. */
/* A002267: nth supersingular prime (primes dividing the Monster group order).   */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The 15 supersingular primes.  They are the primes dividing the Monster
       group order: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 41, 47, 59, 71.       */
    static const uint64_t supersingular_primes[15] = {
        2ULL, 3ULL, 5ULL, 7ULL,
        11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 41ULL,
        47ULL, 59ULL, 71ULL
    };
    static const size_t prime_count = sizeof supersingular_primes / sizeof supersingular_primes[0];

    if (n == 0 || n - 1 >= prime_count) {
        /* Undefined for n < 1 or n > 15; return 0. */
        return 0;
    }

    uint64_t prime = supersingular_primes[n - 1];

    /* Convert the prime to octal, then read those digits as a decimal number. */
    seq_t result = 0;
    uint64_t tmp = prime;
    int digits[32];
    size_t len = 0;

    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp > 0) {
            digits[len++] = (int)(tmp % 8);
            tmp /= 8;
        }
    }

    for (size_t i = 0; i < len; ++i) {
        /* digits stored least‑significant first; process from most significant. */
        result = result * 10 + (seq_t)digits[len - 1 - i];
    }

    return result;
}
