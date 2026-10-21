/*
 * A200726  Define a map f from primes to integers mod 4 by f(p) = 0,1,3,2,1
 *          according as p == 1,2,3,4,0 mod 5; a(n) = Sum_{all primes p}
 *          v_p(n)*f(p), where v_p(n) is the exponent of the highest power of
 *          p dividing n.
 * https://oeis.org/A200726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A200726: a(n) = Σ v_p(n)·f(p)  (mod 4)
 * where f(p) = 0, 1, 3, 2, 1 for primes p ≡ 1, 2, 3, 4, 0 (mod 5),
 * respectively. */
#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;          /* work with an unsigned 64‑bit value */
    seq_t res = 0;

    /* factor 2 separately to speed up odd‑number loops */
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        /* f(2) = 1 (since 2 ≡ 2 mod 5) */
        res = (res + ((seq_t)(e * 1) & 3)) & 3;
    }

    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            uint64_t r = p % 5;
            uint64_t f;
            switch (r) {
                case 1: f = 0; break;   /* p ≡ 1 mod 5 */
                case 2: f = 1; break;   /* p ≡ 2 mod 5 */
                case 3: f = 3; break;   /* p ≡ 3 mod 5 */
                case 4: f = 2; break;   /* p ≡ 4 mod 5 */
                case 0: f = 1; break;   /* p ≡ 0 mod 5 (i.e., p = 5) */
                default: f = 0; break;  /* unreachable */
            }
            res = (res + ((seq_t)(e * f) & 3)) & 3;
        }
    }

    /* if m > 1, it is a remaining prime factor */
    if (m > 1) {
        uint64_t r = m % 5;
        uint64_t f;
        switch (r) {
            case 1: f = 0; break;
            case 2: f = 1; break;
            case 3: f = 3; break;
            case 4: f = 2; break;
            case 0: f = 1; break;
            default: f = 0; break;  /* unreachable */
        }
        res = (res + ((seq_t)(1 * f) & 3)) & 3;
    }

    return res;
}
