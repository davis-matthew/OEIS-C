/*
 * A107760  Expansion of eta(q^3) * eta(q^2)^6 / (eta(q)^3 * eta(q^6)^2) in
 *          powers of q.
 * https://oeis.org/A107760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: A107760 satisfies a(0)=1, and for n>0
 * a(n) = 3 * b(n)
 * where b(n) is multiplicative with
 *   b(p^e) = 1          if p=2 or p=3
 *   b(p^e) = 1 + e      if p ≡ 1 (mod 6)
 *   b(p^e) = 0          if p ≡ 5 (mod 6) and e is odd
 *   b(p^e) = 1          if p ≡ 5 (mod 6) and e is even
 */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    uint64_t m = (uint64_t)n;
    seq_t b = 1;

    /* factor out 2's and 3's – they contribute multiplicative factor 1 */
    while (m % 2 == 0) m /= 2;
    while (m % 3 == 0) m /= 3;

    /* factor the remaining part using the 6k±1 sieve method */
    for (uint64_t p = 5; p * p <= m; p += 6) {
        int e = 0;
        while (m % p == 0) { ++e; m /= p; }
        if (e > 0) {
            if (p % 6 == 1) b *= (seq_t)(1 + e);
            else if (p % 6 == 5) {
                if (e & 1) { b = 0; break; }  /* odd exponent kills b(n) */
            }
        }

        /* check p+2 */
        uint64_t p2 = p + 2;
        if (p2 * p2 > m) { /* no need to check if p2 has exceeded sqrt(m) */
            if (m == p2) {
                if (p2 % 6 == 1) b *= (seq_t)2;
                else if (p2 % 6 == 5) b = 0;
            } else break;
        }
        e = 0;
        while (m % p2 == 0) { ++e; m /= p2; }
        if (e > 0) {
            if (p2 % 6 == 1) b *= (seq_t)(1 + e);
            else if (p2 % 6 == 5) {
                if (e & 1) { b = 0; break; }
            }
        }
    }

    if (b == 0) return (seq_t)0;          /* zero factor found */

    /* remaining prime factor, if any */
    if (m > 1) {
        uint64_t p = m;
        if (p == 2 || p == 3) {
            /* contribution is 1 – nothing to do */
        } else if (p % 6 == 1) {
            b *= (seq_t)2;           /* exponent 1 → 1+1 */
        } else if (p % 6 == 5) {
            /* exponent is 1 → odd → zero */
            return (seq_t)0;
        }
    }

    return b * (seq_t)3;
}
