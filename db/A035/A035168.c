/*
 * A035168  a(n) = Sum_{d|n} Kronecker(-22, d).
 * https://oeis.org/A035168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A035168
   Formula: a(n) = Sum_{d|n} Kronecker(-22, d)
   The sequence is multiplicative.
   For p^e  (p prime):
     a(p^e) = 1                     if p == 2 or p == 11
     a(p^e) = e + 1                 if Kronecker(-22, p) = +1
     a(p^e) = (1 + (-1)^e)/2        if Kronecker(-22, p) = -1
   This implementation factors n by trial division and applies the
   above formula.  It is fast enough for n up to at least 10 000.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"   /* jacobi_symbol(uint64_t a, uint64_t n) */

static int legendre_minus22_prime(uint64_t p) {
    /* (−1/p) */
    int l1 = (p & 3) == 1 ? 1 : -1;
    /* (2/p) */
    int l2 = ((p & 7) == 1 || (p & 7) == 7) ? 1 : -1;
    /* (11/p) via Jacobi symbol */
    int l11 = jacobi_symbol(11, p);      /* returns ±1 for odd prime p          */
    return l1 * l2 * l11;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* zero is undefined for this sequence */
    if (n == 1) return 1;                  /* a(1) = 1 by definition */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* factor n by trial division */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        uint64_t e = 0;
        while (m % p == 0) {
            ++e;
            m /= p;
        }

        /* compute factor for this prime power */
        seq_t f = 1;
        if (p == 2 || p == 11) {
            /* a(p^e) = 1 for p = 2 or 11 */
        } else {
            int kr = legendre_minus22_prime(p);
            if (kr == 1) {
                f = (seq_t)(e + 1);
            } else {                  /* kr == -1 */
                f = (e & 1) == 0 ? (seq_t)1 : (seq_t)0;
            }
        }
        result *= f;
        if (result == 0) return 0;      /* early exit if factor is 0 */
    }

    /* remaining prime factor if any */
    if (m > 1) {
        uint64_t p = m;
        uint64_t e = 1;
        seq_t f = 1;
        if (p == 2 || p == 11) {
            /* a(p) = 1 for p = 2 or 11 */
        } else {
            int kr = legendre_minus22_prime(p);
            if (kr == 1) {
                f = (seq_t)(e + 1);
            } else {
                f = (e & 1) == 0 ? (seq_t)1 : (seq_t)0;
            }
        }
        result *= f;
    }

    return result;
}
