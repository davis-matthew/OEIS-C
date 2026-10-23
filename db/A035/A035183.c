/*
 * A035183  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -5.
 * https://oeis.org/A035183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-5,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/jacobi_symbol.h"

/* Computes a(n) for A035183 */
seq_t a(idx_t n)
{
    if (n == 1) return 1;           /* Kronecker(-5,1) = 1 */

    idx_t remaining = n;
    seq_t ans = 1;

    /* handle factor 2 separately */
    size_t e2 = 0;
    while (remaining % 2 == 0) {
        remaining /= 2;
        ++e2;
    }
    if (e2 % 2 == 1) return 0;       /* a(2^e) = 0 when e is odd */

    /* trial division for odd primes up to sqrt(remaining) */
    for (idx_t p = 3; p * p <= remaining; p += 2) {
        if (remaining % p != 0) continue;

        size_t e = 0;
        while (remaining % p == 0) {
            remaining /= p;
            ++e;
        }

        seq_t contrib;
        if (p == 5) {
            contrib = 1;                     /* a(5^e) = 1 */
        } else {
            /* Kronecker(-5,p) = (−1|p)*(5|p) where (−1|p) = ±1 */
            int minus_one = (p % 4 == 1) ? 1 : -1;
            int five_val  = jacobi_symbol(5, p);   /* (5|p) = ±1 */
            int sym = minus_one * five_val;        /* = ±1 */

            if (sym == 1) {
                contrib = (seq_t)(e + 1);          /* a(p^e) = e+1 */
            } else { /* sym == -1 */
                contrib = (e % 2 == 0) ? (seq_t)1 : (seq_t)0; /* a(p^e) = 1 if e even, else 0 */
            }
        }
        ans *= contrib;
        if (ans == 0) return ans;               /* early exit on zero */
    }

    /* any remaining prime factor > sqrt(original n) */
    if (remaining > 1) {
        idx_t p = remaining;
        seq_t contrib;
        if (p == 5) {
            contrib = 1;
        } else {
            int minus_one = (p % 4 == 1) ? 1 : -1;
            int five_val  = jacobi_symbol(5, p);
            int sym = minus_one * five_val;
            if (sym == 1) {
                contrib = (seq_t)(1 + 1);        /* exponent e = 1 → e+1 */
            } else {
                contrib = (1 % 2 == 0) ? (seq_t)1 : (seq_t)0;
            }
        }
        ans *= contrib;
    }

    return ans;
}
