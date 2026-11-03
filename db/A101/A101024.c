/*
 * A101024  Table of numerators of coefficients of certain rational
 *          polynomials.
 * https://oeis.org/A101024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabl, easy
 * Formula:   a(n) = numerator of the nth element in row‑major order of the *
 *            triangle r(n,m) = (2^m) * C(n,m) * C(n+1,m) / C(2m,m)
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-01)
 * Helpers:   binomial, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* --- locate the row r and column m for the overall index n --- */
    /* S(k) = (k+1)(k+2)/2 is the cumulative size up to row k. */
    double d = sqrt((double)(8 * (unsigned long long)n + 1));
    idx_t r  = (idx_t)((d - 1.0) / 2.0);             /* initial guess */
    
    /* correct possible rounding errors */
    __uint128_t prev = (__uint128_t)r * (r + 1) / 2;  /* S(r-1) */
    while ((idx_t)prev > n) {
        --r;
        prev = (__uint128_t)r * (r + 1) / 2;
    }
    __uint128_t curr = (__uint128_t)(r + 1) * (r + 2) / 2;  /* S(r)   */
    while ((idx_t)curr <= n) {
        ++r;
        curr = (__uint128_t)(r + 1) * (r + 2) / 2;
    }
    idx_t m = n - (idx_t)prev;   /* column within the row */

    /* --- compute the numerator of r(r,m) --- */
    seq_t num = binomial(r, m);          /* C(r,m)   */
    num *= binomial(r + 1, m);           /* *C(r+1,m) */
    num *= pow2(m);                   /* *2^m    */

    /* --- denominator --- */
    seq_t den = binomial(2 * m, m);

    /* --- reduce to lowest terms and return the numerator --- */
    seq_t g = gcd(num, den);
    num /= g;
    return num;
}
