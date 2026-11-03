/*
 * A109064  Expansion of eta(q)^5 / eta(q^5) in powers of q.
 * https://oeis.org/A109064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 28, i16 n <= 6060, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9924 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A109064
   Formula: a(n) = -5 * b(n)  for n > 0, a(0)=1
   where b is multiplicative with
   b(1)=1,   b(5^e)=1,
   and for an odd prime p≠5 (p=2 allowed) and p^e (e≥1):
          b(p)   =  ( Kronecker(5,p) ==  1 ) ? (p+1) : -(p-1)
          b(p^e) =  b(p) * b(p^(e-1))  -  Kronecker(5,p) * p * b(p^(e-2))
*/

#include <stdint.h>

static inline int kronecker_5(idx_t p) {
    /* Kronecker symbol (5|p).  For p=2 it equals -1. */
    if (p == 2) return -1;
    int r = p % 5;
    return (r == 1 || r == 4) ? 1 : -1;
}

/* compute the multiplicative function b(n) for n>0 */
static seq_t b_of_n(idx_t n) {
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* handle factor 2 separately */
    if (m % 2 == 0) {
        int exp = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++exp;
        }
        if (exp > 0) {
            int kr = kronecker_5(2);          /* = -1 */
            seq_t base = (kr == 1) ? (seq_t)(2 + 1) : -(seq_t)(2 - 1); /* -(2-1) = -1 */
            seq_t be = base;
            if (exp > 1) {
                seq_t prev2 = (seq_t)1;       /* b(2^0) */
                seq_t prev1 = base;           /* b(2^1) */
                for (int e = 2; e <= exp; ++e) {
                    seq_t cur = base * prev1 - (seq_t)kr * (seq_t)2 * prev2;
                    prev2 = prev1;
                    prev1 = cur;
                }
                be = prev1;
            }
            result *= be;
        }
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (p == 5) {
                /* b(5^e) = 1, nothing to multiply */
                continue;
            }
            int kr = kronecker_5(p);
            seq_t base = (kr == 1) ? (seq_t)(p + 1) : -(seq_t)(p - 1);
            seq_t be = base;
            if (exp > 1) {
                seq_t prev2 = (seq_t)1;       /* b(p^0) */
                seq_t prev1 = base;           /* b(p^1) */
                for (int e = 2; e <= exp; ++e) {
                    seq_t cur = base * prev1 - (seq_t)kr * (seq_t)p * prev2;
                    prev2 = prev1;
                    prev1 = cur;
                }
                be = prev1;
            }
            result *= be;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        /* m is a prime > 2 */
        if (m == 5) {
            /* b(5) = 1, nothing to multiply */
            return result;
        }
        int kr = kronecker_5(m);
        seq_t base = (kr == 1) ? (seq_t)(m + 1) : -(seq_t)(m - 1);
        result *= base;   /* exponent is 1 */
    }

    return result;
}

typedef uint64_t idx_t;   // index type supplied by the build
/* seq_t is defined by the build; do not redefine it here */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;     /* a(0)=1 */
    seq_t bval = b_of_n(n);
    return - (seq_t)5 * bval;
}
