/*
 * A070682  Smallest m in range 1..phi(2n+1) such that 10^m == 1 mod 2n+1, or
 *          0 if no such number exists.
 * https://oeis.org/A070682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   pow_mod_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A070682: Smallest m in 1..phi(2n+1) with 10^m ≡ 1 mod (2n+1),
   or 0 if no such m exists. */
#include <stdint.h>
#include "../utils/pow_mod_64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* defined as 0 in the OEIS file */

    uint64_t p = 2ULL * (uint64_t)n + 1ULL;  /* modulus: 2n+1, always odd */
    if (p % 5ULL == 0ULL) return 0;          /* gcd(10,p)!=1  →  no order */

    /* compute φ(p) */
    uint64_t phi = p;
    uint64_t temp = p;
    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0ULL) {
            while (temp % d == 0ULL) temp /= d;
            phi = (phi / d) * (d - 1ULL);
        }
    }
    if (temp > 1ULL) {
        phi = (phi / temp) * (temp - 1ULL);
    }

    /* multiplicative order of 10 modulo p, within [1,phi] */
    uint64_t order = phi;
    uint64_t t = order;

    for (uint64_t q = 2; q * q <= t; ++q) {
        if (t % q == 0ULL) {
            while (t % q == 0ULL) t /= q;
            while (order % q == 0ULL &&
                   pow_mod_64(10ULL, order / q, p) == 1ULL)
                order /= q;
        }
    }
    if (t > 1ULL) {   /* remaining prime factor */
        while (order % t == 0ULL &&
               pow_mod_64(10ULL, order / t, p) == 1ULL)
            order /= t;
    }

    return (seq_t)order;
}
