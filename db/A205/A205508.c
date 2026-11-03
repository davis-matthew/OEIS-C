/*
 * A205508  a(n) = Pell(n) * A004018(n) for n>=1 with a(0)=1, where A004018(n)
 *          is the number of ways of writing n as a sum of 2 squares.
 * https://oeis.org/A205508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pell(n) * A004018(n) for n>=1 (a(0)=1) * Pell(n) :
 *            Pell(0)=0, Pell(1)=1, Pell(n)=2*Pell(n-1)+Pell(n-2) *
 *            A004018(n): number of (ordered) representations of n as a^2 +
 *            b^2 * (n=0 → 1 representation: 0^2+0^2) * Implementation uses
 *            the classical factorisation formula: * if n contains a prime...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Computes A004018(n): number of ordered representations of n as a^2 + b^2 */
static seq_t representations_of_two_squares(idx_t n) {
    if (n == 0) return (seq_t)1;  /* only 0^2 + 0^2 */

    seq_t prod = 1;          /* product over primes ≡1 mod 4 of (e+1) */
    idx_t m = n;
    for (idx_t p = 2; (seq_t)p * p <= (seq_t)m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            if ((p & 3) == 1) {          /* p ≡ 1 (mod 4) */
                prod *= (seq_t)(exp + 1);
            } else if ((p & 3) == 3) {   /* p ≡ 3 (mod 4) */
                if (exp & 1) return (seq_t)0;  /* odd exponent → no reps */
            }
            /* p ≡ 2 (mod 4) or p=2: ignore */
        }
    }
    /* remaining prime factor m > 1 */
    if (m > 1) {
        if ((m & 3) == 1) {          /* p ≡ 1 (mod 4) */
            prod *= (seq_t)2;        /* exponent is 1 → factor (1+1)=2 */
        } else if ((m & 3) == 3) {   /* p ≡ 3 (mod 4) */
            return (seq_t)0;         /* exponent odd → no reps */
        }
        /* m ≡ 2 (mod 4) or m=2: ignore */
    }

    return prod * (seq_t)4;          /* final multiplier */
}

/* Forward declare helper for integer square root (may be provided).
 * If not available, a simple implementation is provided later.
 */
#ifndef _floor_isqrt_u64
static inline idx_t _floor_isqrt_u64(idx_t n) {
    idx_t r = 0;
    for (idx_t bit = ((idx_t)1 << ((sizeof(idx_t) * 8 - 1) & ~1)); bit; bit >>= 2) {
        idx_t next = r + bit;
        if (next <= n / next) r = next;
    }
    return r;
}
#endif

/* Computes the n-th Pell number */
static seq_t pell(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t p0 = 0, p1 = 1, pn = 0;
    for (idx_t i = 2; i <= n; ++i) {
        pn = 2 * p1 + p0;
        p0 = p1;
        p1 = pn;
    }
    return pn;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t p = pell(n);
    seq_t r = representations_of_two_squares(n);
    return p * r;
}
