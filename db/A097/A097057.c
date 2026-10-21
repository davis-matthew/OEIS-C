/*
 * A097057  Number of integer solutions to a^2 + b^2 + 2*c^2 + 2*d^2 = n.
 * https://oeis.org/A097057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * ∏_{p^e∥n} b(p^e) where b(2) = 2, b(2^e) = 6 for e>1,
 *            b(p^e) = 1 + p + p^2 + … + p^e for p odd prime
 * Range:     u8 n <= 35, u16 n <= 5939, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation is based on the multiplicative description
   of A097057.  It factorises n (fast enough for n≤10⁴ and even
   larger values), computes the local factor at each prime power,
   multiplies them together, and finally multiplies by 4.  For
   n = 0 the function returns 1 as required. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    uint64_t m = (uint64_t)n;
    seq_t prod = 1;

    /* Handle the factor 2 first.  */
    if ((m & 1ULL) == 0) {
        unsigned e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        if (e == 1)
            prod *= (seq_t)2;
        else if (e > 1)
            prod *= (seq_t)6;
    }

    /* Factor the remaining odd part of m.  */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* Compute 1 + p + p^2 + ... + p^exp.  */
            seq_t term = 1;
            seq_t sum  = 1;
            for (unsigned i = 0; i < exp; ++i) {
                term *= (seq_t)p;
                sum  += term;
            }
            prod *= sum;
        }
    }

    /* If a prime factor larger than sqrt(m) is left.  */
    if (m > 1) {
        /* This factor appears only to the first power.  */
        prod *= (seq_t)(1 + m);
    }

    return prod * (seq_t)4;
}
