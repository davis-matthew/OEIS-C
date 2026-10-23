/*
 * A126851  a(n) = f(n) * Sum_{d|n} f(d) * d, where f(n) =
 *          (-1)^(A055631(A000265(n))/2).
 * https://oeis.org/A126851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = (2^(r+1)-1) * Π_{p odd} [ (p^(e+1)-p)/(p-1) +
 *            (-1)^((p-1)/2) ] where n = 2^r * ∏ p^e is the prime
 *            factorisation. This implements the multiplicative definition of
 *            OEIS A126851. The function relies on the helper pow_uint from
 *            the utils library for integer exponentiation
 * Range:     u8 n <= 16, u16 n <= 2809, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

static seq_t prime_contribution(uint64_t p, int exp)
{
    /* (p^(exp+1) - p) / (p-1)  +  (-1)^((p-1)/2)  (p odd prime) */
    seq_t num = pow_uint(p, exp + 1) - (seq_t)p;
    seq_t denom = (seq_t)(p - 1);
    seq_t geom = num / denom;             /* geometric sum, integer division */
    if (p % 4 == 1)                        /* (-1)^((p-1)/2)=+1 for p≡1 (mod4) */
        geom += 1;
    else                                    /* -1 for p≡3 (mod4) */
        geom -= 1;
    return geom;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1; behaviour for n<1 is irrelevant */
        return 0;

    idx_t tmp = n;
    seq_t result = 1;

    /* factor 2 separately (exponent r) */
    int exp2 = 0;
    while ((tmp & 1ULL) == 0) {          /* while even */
        exp2++;
        tmp >>= 1;
    }
    if (exp2 > 0) {
        seq_t term2 = pow_uint(2ULL, exp2 + 1) - 1;  /* 2^(exp2+1) - 1 */
        result *= term2;
    }

    /* factor odd primes */
    idx_t i = 3;
    while (i * i <= tmp) {
        if (tmp % i == 0) {
            int exp = 0;
            while (tmp % i == 0) {
                exp++;
                tmp /= i;
            }
            result *= prime_contribution((uint64_t)i, exp);
        }
        i += 2;
    }

    /* any remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        result *= prime_contribution((uint64_t)tmp, 1);
    }

    return result;
}
