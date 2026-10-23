/*
 * A008442  Expansion of Jacobi theta constant (theta_2(2z))^2/4.
 * https://oeis.org/A008442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements A008442:
   a(1) = 1
   a(n) = 0 if n is even.
   For each odd prime power p^e dividing n (p odd):
     • If p ≡ 1 (mod 4) then multiply the running product by (1 + e).
     • If p ≡ 3 (mod 4) then a(n) = 0 if e is odd; otherwise the
       product is unchanged.
   The result of the multiplicative process is returned as seq_t. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 */
    if (n == (idx_t)1) return (seq_t)1;

    /* Even numbers give zero */
    if ((n & (idx_t)1) == (idx_t)0) return (seq_t)0;

    seq_t result = (seq_t)1;
    idx_t m = n;           /* mutable copy for factorisation */

    /* Trial‑divide by odd primes up to sqrt(m) */
    for (idx_t p = (idx_t)3; p * p <= m; p += (idx_t)2) {
        if (m % p == (idx_t)0) {
            idx_t e = (idx_t)0;
            while (m % p == (idx_t)0) {
                m /= p;
                ++e;
            }
            if ((p & (idx_t)3) == (idx_t)1) {          /* p ≡ 1 mod 4 */
                result *= (seq_t)(1 + e);
            } else {                                   /* p ≡ 3 mod 4 */
                if ((e & (idx_t)1) != (idx_t)0)        /* odd exponent */
                    return (seq_t)0;
                /* even exponent → factor 1, do nothing */
            }
        }
    }

    /* If a prime factor > sqrt(original n) remains, it has exponent 1 */
    if (m > (idx_t)1) {
        /* m is now a prime, e = 1 */
        idx_t p = m;
        if ((p & (idx_t)3) == (idx_t)1) {              /* p ≡ 1 mod 4 */
            result *= (seq_t)(1 + (idx_t)1);
        } else {                                       /* p ≡ 3 mod 4 */
            return (seq_t)0; /* exponent 1 is odd */
        }
    }

    return result;
}
