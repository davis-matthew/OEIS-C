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
 * Validated: standard, pass, 97 terms through n = 9685 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

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
