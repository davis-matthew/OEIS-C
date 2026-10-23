/*
 * A167357  Totally multiplicative sequence with a(p) = (p-2)*(p+3) = p^2+p-6
 *          for prime p.
 * https://oeis.org/A167357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p|n} ((p-2)*(p+3))^{v_p(n)}. Since 2 divides any even
 *            n, a(n)=0 for even n
 * Range:     u8 n <= 16, u16 n <= 246, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/smallest_prime_factor.h"

/* Helper: fast exponentiation for unsigned types */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 in the OEIS entry */
    if (n == 1)
        return 1;

    /* any even number contains a factor 2 => (2-2)*(2+3)=0, so terminate */
    if ((n & 1) == 0)
        return 0;

    seq_t prod = 1;
    uint64_t m = (uint64_t)n;           /* use 64‑bit arithmetic for factoring */

    /* factor n by repeatedly extracting the smallest prime factor */
    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        idx_t   cnt = 0;
        while (m % p == 0) {
            m /= p;
            ++cnt;
        }
        seq_t base = (seq_t)(p - 2);
        base *= (seq_t)(p + 3);          /* base = (p-2)*(p+3) = p²+p-6 */
        prod *= pow_seq(base, cnt);
    }

    return prod;
}
