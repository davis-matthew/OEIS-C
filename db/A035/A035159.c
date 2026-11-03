/*
 * A035159  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -31.
 * https://oeis.org/A035159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-31,d) The function is
 *            multiplicative: a(p^e) = e+1 if Kronecker(-31,p) = 1 a(p^e) =
 *            (1+(-1)^e)/2 if Kronecker(-31,p) = -1 with a(31^e) = 1. The code
 *            uses factorisation by repeated smallest‑prime‑factor extraction
 *            and evaluates the Kronecker symbol via the Jacobi symbol
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9115 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/smallest_prime_factor.h"

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

seq_t a(idx_t n)
{
    /* a(1) = 1;  for n >= 1 */
    if (n == 0) return 0;
    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        uint64_t exp = 0;
        do {
            m /= p;
            exp++;
        } while (m % p == 0);

        seq_t contrib;

        /* handle the special prime 31 (Kronecker(-31,31)=0) */
        if (p == 31ULL) {
            contrib = 1;
        } else {
            int kronecker;  /* -1, 0, or 1 */

            /* special case p = 2 */
            if (p == 2ULL) {
                kronecker = 1;
            } else {
                /* compute (−31|p) = (−1|p)*(31|p) */
                uint64_t mod31 = 31 % p;
                int leg1 = jacobi_symbol(mod31, p);        /* (31|p) */
                int leg2 = ((p & 3) == 1) ? 1 : -1;       /* (−1|p) */
                kronecker = leg1 * leg2;
            }

            if (kronecker == 1) {
                contrib = (seq_t)(exp + 1);
            } else { /* kronecker == -1 */
                contrib = (exp % 2 == 0) ? (seq_t)1 : (seq_t)0;
            }
        }
        result *= contrib;
    }
    return result;
}
