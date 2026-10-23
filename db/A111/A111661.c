/*
 * A111661  Expansion of eta(q)^4 * eta(q^2) * eta(q^6)^5 / eta(q^3)^4 in
 *          powers of q.
 * https://oeis.org/A111661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 16, i32 n <= 1761, i64 n <= 10000,
 *            i128 n <= 3, big any n
 * Helpers:   mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A111661: Multiplicative function
   a(2^e)   = (-4)^e
   a(3^e)   = 1
   a(p^e)   = ((p^2)^(e+1)-1)/(p^2-1)    if p ≡ 1 (mod 6)
   a(p^e)   = (1-(-p^2)^(e+1))/(p^2+1)   if p ≡ 5 (mod 6)   */
/* Uses mul128 helper for exact multiplication with seq_t types */

#include <stdint.h>
#include "../utils/mul128.h"   /* declares seq_t mul128(seq_t a, seq_t b); */

static seq_t pow_seq_int(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b      = base;
    while (exp) {
        if (exp & 1) result = mul128(result, b);
        b   = mul128(b, b);
        exp >>= 1;
    }
    return result;
}

/* local contribution of a prime p with exponent e */
static seq_t local_contrib(uint64_t p, uint64_t e)
{
    if (p == 2) {
        seq_t v = 1;
        for (uint64_t i = 0; i < e; ++i) v = mul128(v, -4);
        return v;
    }
    if (p == 3) return 1;   /* a(3^e) = 1 for all e */

    seq_t mod = (seq_t)p * (seq_t)p;   /* p^2 */

    if (p % 6 == 1) {
        /* (mod^(e+1)-1)/(mod-1)  = 1 + mod + mod^2 + ... + mod^e */
        seq_t num = pow_seq_int(mod, e + 1) - 1;
        seq_t den = mod - 1;
        return num / den;   /* division is exact */
    } else {   /* p % 6 == 5 */
        /* (1-(-mod)^(e+1))/(mod+1)  = 1 - mod + mod^2 - ... +/- mod^e */
        seq_t num = 1 - pow_seq_int(-mod, e + 1);
        seq_t den = mod + 1;
        return num / den;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, return 0 for safety */
    seq_t result = 1;
    idx_t m = n;

    /* handle factor 2 */
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        result = mul128(result, local_contrib(2, e));
    }

    /* trial division for remaining factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result = mul128(result, local_contrib(p, e));
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (m > 1) {
        result = mul128(result, local_contrib(m, 1));
    }

    return result;
}
