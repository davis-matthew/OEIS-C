/*
 * A035164  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -26.
 * https://oeis.org/A035164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Σ_{d|n} Kronecker(-26,d) Computation uses
 *            multiplicativity: a(p^e) = 1 if Kronecker(-26,p)=0 (p=2 or 13)
 *            (1+(-1)^e)/2 (=0 if e odd) if Kronecker(-26,p)=-1 e+1 if
 *            Kronecker(-26,p)= 1 The Kronecker symbol for a prime p is
 *            obtained from the Jacobi symbol
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

/* helper: Kronecker(-26,p) for a prime p. */
static int kronecker_minus26_prime(idx_t p)
{
    if (p == 2 || p == 13) return 0; /* (-26/p)=0 when p divides 26 */

    /* (-1/p) = 1 if p ≡ 1 (mod 4), else -1. */
    int neg_one = (p % 4 == 1) ? 1 : -1;

    /* (26/p) via Jacobi symbol (valid for p∤26). */
    int k26 = jacobi_symbol(26, p);   /* returns ±1 */

    return neg_one * k26;   /* (-26/p) = (-1/p)*(26/p) */
}

seq_t a(idx_t n)
{
    seq_t res = 1;
    idx_t temp = n;
    idx_t p;

    /* factor 2 separately */
    if (temp % 2 == 0) {
        int e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++e;
        }
        /* Kronecker(-26,2)=0 → part = 1, so nothing to do */
    }

    /* iterate over odd primes */
    for (p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            int k = kronecker_minus26_prime(p);
            seq_t part;
            if (k == 0) {
                part = 1;                         /* a(p^e)=1 */
            } else if (k == -1) {
                part = (e & 1) ? (seq_t)0 : (seq_t)1;
            } else { /* k == 1 */
                part = (seq_t)e + 1;
            }
            res *= part;
            if (res == 0) return 0;            /* early exit if zero occurs */
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        idx_t p = temp;        /* exponent e = 1 */
        int k = kronecker_minus26_prime(p);
        seq_t part;
        if (k == 0) {
            part = 1;
        } else if (k == -1) {
            part = 0;           /* e is odd → part = 0 */
        } else { /* k == 1 */
            part = 2;           /* e+1 = 2 */
        }
        res *= part;
    }

    return res;
}
