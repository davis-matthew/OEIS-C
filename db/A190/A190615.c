/*
 * A190615  Expansion of f(x^2) * f(x^3) / (chi(x) * chi(x^6)) in powers of x
 *          where f(), chi() are Ramanujan theta functions.
 * https://oeis.org/A190615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2*n + 1) where b is the multiplicative function
 *            specified in the OEIS entry: b(p^e) = 1 if p = 3 and e even,
 *            b(p^e) = -1 if p = 3 and e odd, b(p^e) = e+1 if p ≡ 1 or 5 (mod
 *            24), b(p^e) = (-1)^e*(e+1) if p ≡ 7 or 11 (mod 24), b(p^e) = 1
 *            if p ≡ 13,17,19,23 (mod 24) and e even, b(p^e) = 0 i...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the prime‑power contribution to b(p^e). */
static seq_t prime_contribution(uint64_t p, int e)
{
    if (p == 3) {
        return (e & 1) ? (seq_t)(-1) : (seq_t)1;
    }

    uint64_t r = p % 24;
    if (r == 1 || r == 5) {
        return (seq_t)(e + 1);
    } else if (r == 7 || r == 11) {
        seq_t sign = (e & 1) ? (seq_t)(-1) : (seq_t)1;
        return sign * (seq_t)(e + 1);
    } else if (r == 13 || r == 17 || r == 19 || r == 23) {
        return ((e & 1) == 0) ? (seq_t)1 : (seq_t)0;
    }
    /* p==2 cannot occur because 2*n+1 is odd */
    return (seq_t)1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute m = 2*n + 1.  n is uint64_t, so the product fits
       into unsigned __int128 (guaranteed as we only test n up to 10 000).  */
    unsigned __int128 m128 = ((unsigned __int128)2 * n) + 1;
    uint64_t m = (uint64_t)m128;  /* m is odd for all n ≥ 0 */

    if (m == 1)              /* n = 0 ⇒ m = 1 */
        return (seq_t)1;

    seq_t result = 1;
    uint64_t t = m;

    /* factor out 3 first (special case) */
    int e = 0;
    while ((t % 3) == 0) {
        ++e;
        t /= 3;
    }
    if (e > 0) {
        seq_t val = prime_contribution(3, e);
        if (val == 0)
            return (seq_t)0;
        result *= val;
    }

    /* trial division by odd numbers from 5 upwards */
    for (uint64_t p = 5; p * p <= t; p += 2) {
        e = 0;
        while ((t % p) == 0) {
            ++e;
            t /= p;
        }
        if (e > 0) {
            seq_t val = prime_contribution(p, e);
            if (val == 0)
                return (seq_t)0;
            result *= val;
        }
    }

    /* if anything remains, it is a prime factor > sqrt(m) */
    if (t > 1) {
        e = 1;
        seq_t val = prime_contribution(t, e);
        if (val == 0)
            return (seq_t)0;
        result *= val;
    }

    return result;
}
