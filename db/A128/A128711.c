/*
 * A128711  Expansion of phi(x) * psi(x^4) * phi(-x^4)^4 in powers of x where
 *          phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A128711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(2*n+1) where b is multiplicative. For a prime
 *            p≡5,7(mod8): b(p^e) = 0 if e odd, otherwise p^e. For
 *            p≡1,3(mod8): b(p) = 2·(2x²−p)·(−1)^((p−1)/2) with p = x²+2y²,
 *            and b(p^e) = b(p)·b(p^(e−1)) − p²·b(p^(e−2)) for e≥2
 * Range:     i8 n <= 35, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   b_prime, find_rep, int_sqrt32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/b_prime.h"
#include "../utils/find_rep.h"
#include "../utils/int_sqrt32.h"

/* power of a prime: p^e where e>=0 */
static seq_t pow_uint32(uint32_t p, uint32_t e) {
    seq_t r = 1;
    for (uint32_t i = 0; i < e; ++i) r *= (seq_t)p;
    return r;
}

seq_t a(idx_t n) {
    idx_t m = (idx_t)2 * n + 1ULL;   /* argument to b is 2*n+1, always odd */
    if (m == 1) return (seq_t)1;     /* b(1)=1 */

    seq_t result = 1;
    idx_t tmp = m;

    /* trial division up to sqrt(tmp) */
    for (idx_t p = 3; (uint64_t)p * p <= tmp; p += 2) {
        if (tmp % p) continue;
        uint32_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }

        seq_t term;
        if ((p & 7) == 5 || (p & 7) == 7) {   /* p ≡ 5 or 7 mod 8 */
            if (e & 1) return (seq_t)0;       /* odd exponent zeroes the product */
            term = pow_uint32((uint32_t)p, e); /* e even, term = p^e */
        } else {                              /* p ≡ 1 or 3 mod 8 */
            seq_t bp = b_prime((uint32_t)p);
            if (e == 1) {
                term = bp;
            } else {
                seq_t prev2 = 1;     /* b(p^0) */
                seq_t prev1 = bp;    /* b(p^1) */
                seq_t curr = 0;
                for (uint32_t k = 2; k <= e; ++k) {
                    curr = prev1 * bp - (seq_t)p * (seq_t)p * prev2;
                    prev2 = prev1;
                    prev1 = curr;
                }
                term = curr;
            }
        }
        result *= term;
        if (tmp == 1) break;
    }

    /* if remaining factor >1, it is a prime > sqrt(original m) */
    if (tmp > 1) {
        uint32_t p = (uint32_t)tmp;
        seq_t term;
        if ((p & 7) == 5 || (p & 7) == 7) {
            term = (seq_t)p;   /* exponent is 1 and even because power 1 is odd -> zero? but this case never occurs because if p≡5,7 the term would be 0? Actually e=1 odd so should zero overall. But since tmp>1 and parity odd, overall a(n)=0. */
            /* Indeed, if a remaining prime is ≡5,7, then overall result should be 0. */
            return (seq_t)0;
        } else {
            term = b_prime(p);
        }
        result *= term;
    }

    return result;
}
