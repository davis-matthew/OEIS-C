/*
 * A097109  G.f.: s(2)^2*s(3)^3/(s(1)*s(6)^2), where s(k) = eta(q^k) and
 *          eta(q) is Dedekind's function, cf. A010815.
 * https://oeis.org/A097109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative; a(0)=a(1)=1. For prime powers: * a(2^e)
 *            = -3 if e>0 and e even, 0 if e odd. * a(3^e) = -2 (for any e>0).
 *            * if p≡1 (mod 6): a(p^e) = e+1. * if p≡5 (mod 6): a(p^e) = 1 if
 *            e even, 0 if e odd
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;          /* work in unsigned 64‑bit for safety */
    seq_t  res = (seq_t)1;

    /* factor 2 */
    int e2 = 0;
    while ((m & 1ULL) == 0) {
        e2++; m >>= 1;
    }
    if (e2 > 0) {
        seq_t term = (e2 & 1) ? (seq_t)0 : (seq_t)-3;
        if (term == (seq_t)0) return (seq_t)0;
        res *= term;
    }

    /* factor 3 */
    int e3 = 0;
    while (m % 3ULL == 0) {
        e3++; m /= 3ULL;
    }
    if (e3 > 0) {
        res *= (seq_t)-2;            /* always non‑zero */
    }

    /* factor odd primes >=5 */
    for (uint64_t p = 5ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                e++; m /= p;
            }
            seq_t term;
            if (p % 6ULL == 1) {
                term = (seq_t)(e + 1);          /* e+1 fits in seq_t for tested range */
            } else { /* p % 6 == 5 */
                term = (e & 1) ? (seq_t)0 : (seq_t)1;
            }
            if (term == (seq_t)0) return (seq_t)0;
            res *= term;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        uint64_t p = m;          /* exponent is 1 */
        seq_t term;
        if (p % 6ULL == 1) {
            term = (seq_t)2;      /* e=1 => e+1 = 2 */
        } else { /* p % 6 == 5 */
            term = (seq_t)0;      /* odd exponent => 0 */
        }
        if (term == (seq_t)0) return (seq_t)0;
        res *= term;
    }

    return res;
}
