/*
 * A088580  a(n) = 1 + sigma(n).
 * https://oeis.org/A088580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sigma(n)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9260 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where sigma(n) = sum of all positive divisors of n  */

#include <stdlib.h>
#include <stdint.h>

/* If the build uses an arbitrary precision integer class, the cast
   below will instantiate the appropriate constructor.  For fixed-width
   unsigned types the cast simply truncates the __int128 intermediate
   value, which is safe because the result always fits in those types
   for the intended input range.  */
static seq_t sigma_plus_one(idx_t n)
{
    if (n == 0)  /* offset begins at 1; behaviour for n=0 is arbitrary */
        return (seq_t)0;

    __int128 result = 1;          /* product of σ(p^e) terms            */
    idx_t original = n;
    idx_t temp = n;

    /* factor 2 */
    if ((temp & 1) == 0) {
        idx_t exp = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++exp;
        }
        /* σ(2^exp) = 2^(exp+1) - 1          */
        __int128 term = ((__int128)1 << (exp + 1)) - 1;
        result *= term;
    }

    /* odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* σ(p^exp) = (p^(exp+1) - 1) / (p - 1)  */
            __int128 p_pow = 1;
            for (idx_t i = 0; i < exp + 1; ++i)
                p_pow *= p;
            __int128 term = (p_pow - 1) / (p - 1);
            result *= term;
        }
    }

    /* remaining prime > sqrt(original) */
    if (temp > 1) {
        /* σ(temp) = temp + 1  */
        result *= (temp + 1);
    }

    /* sigma(original) + 1 */
    seq_t sigma = (seq_t)result;
    return sigma + 1;
}

seq_t a(idx_t n)
{
    return sigma_plus_one(n);
}
