/*
 * A063754  Dirichlet convolution of totient and cototient.
 * https://oeis.org/A063754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = Sum_{d|n} phi(d) * ( (n/d) - phi(n/d) )
 * Range:     u8 n <= 83, u16 n <= 5039, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes Euler's totient function phi on the fly. */

#include <stdint.h>
#include <stddef.h>

/* Short‐hand totient for 64‑bit integers, result returned as seq_t */
static seq_t phi_uint64(uint64_t m)
{
    if (m == 0) return (seq_t)0;
    uint64_t result = m;
    uint64_t t = m;

    if ((t & 1ULL) == 0) {
        /* factor 2 */
        while ((t & 1ULL) == 0) t >>= 1;
        result -= result >> 1;           /* result = result * (2-1)/2 */
    }

    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            result -= result / p;        /* result = result * (p-1)/p */
        }
    }

    if (t > 1) {          /* remaining prime factor */
        result -= result / t;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, return 0 for safety */

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            seq_t phi_d1 = phi_uint64(d1);
            seq_t phi_d2 = phi_uint64(d2);

            /* contribution from divisor d1 */
            seq_t cot_d2 = (seq_t)d2 - phi_d2;
            sum += phi_d1 * cot_d2;

            if (d1 != d2) {
                /* contribution from divisor d2 */
                seq_t cot_d1 = (seq_t)d1 - phi_d1;
                sum += phi_d2 * cot_d1;
            }
        }
    }
    return sum;
}
