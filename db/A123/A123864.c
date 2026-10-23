/*
 * A123864  Expansion of (eta(q^3) * eta(q^5))^2 / (eta(q) * eta(q^15)) in
 *          powers of q.
 * https://oeis.org/A123864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   kronecker_minus15_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Multiplicative formula for OEIS A123864:
   a(1)=1 and for n≥1
   a(n)=∏_{p^e‖n} f(p^e)  where  f is defined as follows
     * f(3^e)=f(5^e)=1
     * f(2^e)=e+1
     * for odd primes p≠3,5:
           let K be the Kronecker symbol (−15/p)
           if K =  1 : f(p^e)=e+1
           if K = –1 : f(p^e)= 0  if e is odd, 1 if e is even
*/

#include <stdint.h>
#include "../utils/kronecker_minus15_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* defined in the OEIS entry */

    seq_t result = 1;
    idx_t m = n;

    /* handle factor 2 separately */
    if (m & 1LL) { /* odd */
        /* nothing */
    } else {
        idx_t e = 0;
        while ((m & 1LL) == 0) {
            m >>= 1;
            ++e;
        }
        /* p = 2 */
        result = result * (seq_t)(e + 1);
    }

    /* factorization of remaining odd part */
    idx_t i = 3;
    while (i * i <= m) {
        if (m % i == 0) {
            idx_t e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }

            if (i == 3 || i == 5) {
                /* contributes 1, do nothing */
            } else {
                int k = kronecker_minus15_prime(i);
                if (k == 1) {
                    result = result * (seq_t)(e + 1);
                } else if (k == -1) {
                    if (e & 1)   /* odd exponent gives zero */
                        return 0;
                    /* even exponent gives 1, product unchanged */
                }
            }
        }
        i += 2;
    }

    /* any remaining prime factor > sqrt(m) has exponent 1 */
    if (m > 1) {
        idx_t p = m;
        if (p == 3 || p == 5) {
            /* contributes 1 */
        } else {
            int k = kronecker_minus15_prime(p);
            if (k == 1) {
                result = result * (seq_t)2;   /* e = 1 → e+1 = 2 */
            } else if (k == -1) {
                /* exponent 1 is odd → zero */
                return 0;
            }
        }
    }

    return result;
}
