/*
 * A070921  a(n) = Card{ (x,y,z,u,v) | lcm(x,y,z,u,v)=n }.
 * https://oeis.org/A070921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = ∏_{p^e||n} ((e+1)^5 − e^5) (multiplicative, a(p^e) =
 *            (e+1)^5 − e^5) Offset: 1 (a(1) = 1)
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow5_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow5_small.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* offset 1: a(1) is first term */

    /* a(1) should be 1 */
    if (n == 1) return 1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p = 2;

    /* factor 2 */
    if (m % p == 0) {
        int e = 0;
        while (m % p == 0) { m /= p; ++e; }
        seq_t a1 = (seq_t)(e + 1);
        seq_t a5 = pow5_small(a1);
        seq_t e5 = pow5_small((seq_t)e);
        result *= (a5 - e5);
    }

    /* factor odd primes */
    p = 3;
    while (p * p <= m) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            seq_t a1 = (seq_t)(e + 1);
            seq_t a5 = pow5_small(a1);
            seq_t e5 = pow5_small((seq_t)e);
            result *= (a5 - e5);
        }
        p += 2;
    }

    /* if remainder is prime */
    if (m > 1) {
        /* exponent is 1 */
        seq_t a1 = (seq_t)2;      /* (1+1)=2 */
        seq_t a5 = pow5_small(a1);  /* 2^5 = 32 */
        seq_t e5 = 1;              /* 1^5 = 1 */
        result *= (a5 - e5);       /* 31 */
    }

    return result;
}
