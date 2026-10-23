/*
 * A035229  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s) + Kronecker(m,p)*p^(-2s))^(-1) for m
 *          = 47.
 * https://oeis.org/A035229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(47, d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* For n = 0 the function is undefined; return 0 so as not to crash. */
    if (n == 0) return 0;

    seq_t result = 1;
    uint64_t m = n;
    uint64_t p = 2;

    /* Trial division up to sqrt(m). */
    while (p * p <= m) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }

            seq_t term;
            if (p == 47) {
                term = 1;                               /* a(47^e) = 1 */
            } else {
                int ks = jacobi_symbol(47, p);          /* Kronecker symbol */
                if (ks == 1) {
                    term = (seq_t)exp + 1;              /* a(p^e) = e+1 */
                } else if (ks == -1) {
                    term = (exp & 1) ? 0 : 1;           /* a(p^e) = (1+(-1)^e)/2 */
                } else { /* ks == 0 (should not happen for odd p) */
                    term = 0;
                }
            }

            result *= term;
            if (result == 0) return 0;                  /* early exit for zero */
        }

        /* Advance to the next possible prime. */
        if (p == 2) p = 3;
        else p += 2;
    }

    /* Remaining prime factor > sqrt(n). */
    if (m > 1) {
        uint64_t p = m;
        int exp = 1;                                   /* exponent is 1 */
        seq_t term;
        if (p == 47) {
            term = 1;
        } else {
            int ks = jacobi_symbol(47, p);
            if (ks == 1) {
                term = (seq_t)exp + 1;                  /* 2 */
            } else if (ks == -1) {
                term = (exp & 1) ? 0 : 1;               /* 1 */
            } else {
                term = 0;
            }
        }
        result *= term;
    }

    return result;
}
