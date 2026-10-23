/*
 * A035178  a(n) = Sum_{d|n} Kronecker(-12, d) (= A134667(d)).
 * https://oeis.org/A035178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-12, d) (multiplicative with a(p^e)=1
 *            if p=2 or p=3, a(p^e)=1+e if p≡1 (mod 6), a(p^e)=(1+(-1)^e)/2 if
 *            p≡5 (mod 6))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)            /* by definition a(1)=1 */
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* factor 2 */
    size_t e = 0;
    while ((temp & 1ULL) == 0ULL) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) {               /* p = 2 -> term = 1 */
        result *= (seq_t)1;
    }

    /* factor 3 */
    e = 0;
    while (temp % 3ULL == 0ULL) {
        temp /= 3ULL;
        ++e;
    }
    if (e > 0) {                 /* p = 3 -> term = 1 */
        result *= (seq_t)1;
    }

    /* factor odd primes >=5 */
    for (uint64_t p = 5ULL; p * p <= temp; p += 2ULL) {
        if (temp % p != 0ULL)
            continue;
        e = 0;
        while (temp % p == 0ULL) {
            temp /= p;
            ++e;
        }
        if (p % 6ULL == 1ULL) {          /* p ≡ 1 (mod 6) */
            result *= (seq_t)(1 + e);
            if (result == (seq_t)0)     /* product can become zero */
                return result;
        } else {                         /* p ≡ 5 (mod 6) */
            /* (1 + (-1)^e)/2 equals 1 if e even, 0 if e odd */
            if ((e & 1ULL) == 0ULL) {
                result *= (seq_t)1;
            } else {
                /* multiply by 0 -> result becomes 0 */
                result = (seq_t)0;
                return result;
            }
        }
    }

    /* if remaining temp > 1, it's a prime factor */
    if (temp > 1ULL) {
        uint64_t p = temp;
        /* exponent is 1 */
        if (p % 6ULL == 1ULL) {
            result *= (seq_t)2;            /* 1 + 1 */
        } else {                           /* p % 6 == 5 */
            /* exponent 1 odd -> term 0 */
            result = (seq_t)0;
        }
    }

    return result;
}
