/*
 * A163746  Expansion of (theta_3(q)^2 + 3 * theta_3(q^3)^2) / 4 - 1 in powers
 *          of q.
 * https://oeis.org/A163746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A163746
   Formula: multiplicative, with local factors
     a(2^e)          = 1
     a(3^e)          = 2 - (-1)^e   // = 1 if e even, 3 if e odd
     a(p^e)  (p≡1 mod4) = e+1
     a(p^e)  (p≡3 mod4) = (1+(-1)^e)/2   // = 1 if e even, 0 if e odd
   a(1) = 1
*/

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = 1;
    idx_t temp = n;

    /* handle factor 2 – contribution is always 1 */
    if ((temp & 1ULL) == 0) {
        do { temp >>= 1; } while ((temp & 1ULL) == 0);
    }

    /* handle factor 3 – a(3^e) = 1 if e even, 3 if e odd */
    if (temp % 3 == 0) {
        uint64_t e3 = 0;
        while (temp % 3 == 0) {
            temp /= 3;
            e3++;
        }
        if (e3 & 1ULL)            /* odd exponent */
            res *= (seq_t)3;     /* multiply by 3; even exponent gives factor 1 */
    }

    /* handle all remaining primes >3 via 6k±1 loop */
    idx_t i = 5;
    unsigned step = 2;         /* alternate 2,4 */
    while (i * i <= temp) {
        if (temp % i == 0) {
            uint64_t exp = 0;
            while (temp % i == 0) {
                temp /= i;
                exp++;
            }

            if (i % 4 == 1) {          /* p≡1 (mod4) → factor = exp+1 */
                res *= (seq_t)(exp + 1);
            } else {                   /* p≡3 (mod4) → factor = 1 if exp even, 0 if odd */
                if (exp & 1ULL)         /* odd exponent → factor 0 */
                    return (seq_t)0;
            }
        }
        i += step;
        step = 6 - step;           /* 2,4,2,4,… */
    }

    /* if a prime factor remains > sqrt(n) */
    if (temp > 1) {
        if (temp % 4 == 1)          /* p≡1 (mod4) → exponent is 1 → factor 2 */
            res *= (seq_t)2;
        else if (temp % 4 == 3)     /* p≡3 (mod4) → exponent 1 odd → factor 0 */
            return (seq_t)0;
        /* temp cannot be 2 here – already removed */
    }

    return res;
}
