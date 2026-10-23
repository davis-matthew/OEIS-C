/*
 * A133079  Expansion of f(x)^3 - 3 * x * f(x^9)^3 in powers of x^3 where f()
 *          is a Ramanujan theta function.
 * https://oeis.org/A133079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   For m = 24*n + 1, a(n) = 0 if any prime exponent in m is odd.
 *            Otherwise let m = ∏ p^{2f_p}. Then a(n) = (∏ p^{f_p}) *
 *            (-1)^{∑_{p≡5,7(8)} f_p}
 * Range:     i8 n <= 714, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const uint32_t primes[] = {
    2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U,
    31U, 37U, 41U, 43U, 47U, 53U, 59U, 61U, 67U, 71U,
    73U, 79U, 83U, 89U, 97U, 101U, 103U, 107U, 109U, 113U,
    127U, 131U, 137U, 139U, 149U, 151U, 157U, 163U, 167U, 173U,
    179U, 181U, 191U, 193U, 197U, 199U, 211U, 223U, 227U, 229U,
    233U, 239U, 241U, 251U, 257U, 263U, 269U, 271U, 277U, 281U,
    283U, 293U, 307U, 311U, 313U, 317U, 331U, 337U, 347U, 349U,
    353U, 359U, 367U, 373U, 379U, 383U, 389U, 397U, 401U, 409U,
    419U, 421U, 431U, 433U, 439U, 443U, 449U, 457U, 461U, 463U,
    467U, 479U, 487U
};
static const size_t prime_count = sizeof(primes) / sizeof(primes[0]);

seq_t a(idx_t n)
{
    uint64_t m = 24ULL * (uint64_t)n + 1ULL;
    uint64_t rem = m;
    seq_t product = 1;
    int sign = 1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > rem) break;
        if (rem % p == 0) {
            uint64_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            if (exp & 1ULL) return 0;          /* odd exponent -> zero */
            uint64_t f = exp >> 1;             /* exp / 2 */
            seq_t power = 1;
            for (uint64_t j = 0; j < f; ++j)
                power *= (seq_t)p;
            product *= power;
            if ((p & 7U) == 5U || (p & 7U) == 7U)
                if (f & 1ULL)
                    sign = -sign;
        }
    }

    if (rem > 1) return 0;                     /* remaining prime factor has odd exponent */

    if (sign < 0) product = -product;
    return product;
}
