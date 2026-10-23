/*
 * A028641  Expansion of theta_3(q) * theta_3(q^19) + theta_2(q) *
 *          theta_2(q^19) in powers of q.
 * https://oeis.org/A028641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   kronecker_minus19, legendre_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028641 – Expansion of theta_3(q) * theta_3(q^19) + theta_2(q) * theta_2(q^19)
   Formula: a(n) = 2 * ∏_{p^e∥n} f(p,e) with
            f(19,e) = 1,
            f(p,e) = e+1            if Kronecker(-19,p)=+1,
            f(p,e) = (1+(-1)^e)/2   if Kronecker(-19,p)=-1. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/kronecker_minus19.h"
#include "../utils/legendre_symbol.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;      /* a(0) = 1 */

    uint64_t tmp = (uint64_t)n;
    seq_t prod = (seq_t)1;

    /* factor n by trial division */
    if ((tmp & 1) == 0) {               /* handle factor 2 separately? */
        int exp = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++exp;
        }
        /* Kronecker(-19,2) = -1 : odd exponent => zero, even => factor 1 */
        if ((exp & 1) == 0) {
            /* nothing, multiplier is 1 */
        } else {
            return (seq_t)0;   /* product is zero */
        }
    }

    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (p == 19) {
                /* factor 1, nothing to do */
                continue;
            }
            int k = kronecker_minus19(p);
            if (k == 1) {
                prod = prod * (seq_t)(exp + 1);
            } else { /* k == -1 */
                if ((exp & 1) == 1) {
                    return (seq_t)0;   /* odd exponent gives zero */
                }
                /* even exponent gives factor 1 */
            }
        }
    }

    /* If a prime factor > sqrt remains */
    if (tmp > 1) {
        uint64_t p = tmp;
        int exp = 1;               /* exponent is 1 */
        if (p == 19) {
            /* factor 1, nothing */
        } else {
            int k = kronecker_minus19(p);
            if (k == 1) {
                prod = prod * (seq_t)(exp + 1);  /* exp is 1 -> 2 */
            } else { /* k == -1 */
                if ((exp & 1) == 1) {
                    return (seq_t)0;   /* odd exponent gives zero */
                }
                /* even exponent gives factor 1 */
            }
        }
    }

    return prod * (seq_t)2;
}
