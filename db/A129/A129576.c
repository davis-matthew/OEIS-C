/*
 * A129576  Expansion of phi(x) * chi(x) * psi(-x^3) in powers of x where
 *          phi(), chi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A129576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = multiplicative function b(3*n+1) where:
   • b(2^e) = 3 if e even else 0
   • b(p^e) = e+1      if p ≡ 1 (mod 3)
   • b(p^e) = 0 if p ≡ 2 (mod 3) and e odd, 1 if e even
   • b(3^e) = 0 for e>0 (never needed because 3*n+1 ≡ 1 (mod 3))
   The sequence A129576 is defined as a(n) = b(3*n+1).  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case early */
    if (n == 0)
        return (seq_t)1;

    /* Compute m = 3*n + 1, guaranteed to fit in uint64_t for
       the intended range of n (even for n up to 10^12). */
    uint64_t m = 3ULL * (uint64_t)n + 1ULL;

    seq_t result = 1;

    /* Handle factor 2 */
    if ((m & 1ULL) == 0) {
        unsigned e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            e++;
        }
        if ((e & 1) == 0) {           /* e even */
            result *= (seq_t)3;
        } else {                      /* e odd -> factor 0 -> product 0 */
            return (seq_t)0;
        }
    }

    /* Handle factor 3 (not really needed as m ≡ 1 (mod 3), but for
       completeness). */
    if (m % 3ULL == 0) {
        unsigned e = 0;
        while (m % 3ULL == 0) {
            m /= 3ULL;
            e++;
        }
        /* b(3^e) = 0 for e>0 */
        if (e > 0)
            return (seq_t)0;
    }

    /* Trial division for odd primes starting at 5. */
    for (uint64_t p = 5ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0ULL) {
            unsigned e = 0;
            while (m % p == 0ULL) {
                m /= p;
                e++;
            }

            seq_t factor;
            if (p % 3ULL == 1ULL) {
                factor = (seq_t)(e + 1);
            } else { /* p % 3 == 2 */
                factor = (e & 1) == 0 ? (seq_t)1 : (seq_t)0;
            }

            if (factor == 0)
                return (seq_t)0;
            result *= factor;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1ULL) {
        uint64_t p = m;      /* exponent e = 1 */
        seq_t factor;
        if (p % 3ULL == 1ULL) {
            factor = (seq_t)2;   /* e+1 = 2 */
        } else { /* p % 3 == 2 */
            factor = (seq_t)0;   /* (1+(-1)^1)/2 = 0 */
        }
        if (factor == 0)
            return (seq_t)0;
        result *= factor;
    }

    return result;
}
