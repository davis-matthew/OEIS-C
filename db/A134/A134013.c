/*
 * A134013  Expansion of q * phi(q) * psi(q^8) in powers of q where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A134013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A134013
   Formula: a(n) is multiplicative with 
       a(2)          = 2,   a(2^e) = 0 for e > 1
       a(p^e)        = e + 1          if p ≡ 1 (mod 4)
       a(p^e)        = (1 + (-1)^e)/2 if p ≡ 3 (mod 4)
   e.g. a(1) = 1.  For n having any prime power factor that makes the
   factor 0 the whole product is 0.
*/
#include <stdint.h>

/* small integer square‑root for 64‑bit values */
static inline uint64_t int_sqrt_u64(uint64_t n)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;          /* highest power of four ≤ n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* multiplicative function a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined below offset */
    seq_t result = 1;
    uint64_t m = n;
    uint64_t exp;

    /* factor 2 separately */
    exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1ULL;
        ++exp;
    }
    if (exp > 0) {
        if (exp == 1) {
            result *= (seq_t)2;
        } else {
            return (seq_t)0;                   /* 2^e with e>1 gives zero */
        }
    }

    /* factor odd primes */
    uint64_t limit = int_sqrt_u64(m);
    for (uint64_t p = 3; p <= limit; p += 2) {
        if (m % p != 0) continue;
        exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        seq_t factor;
        if ((p & 3ULL) == 1ULL) {             /* p ≡ 1 (mod 4) */
            factor = (seq_t)(exp + 1);
        } else {                              /* p ≡ 3 (mod 4) */
            factor = (exp & 1ULL) ? (seq_t)0 : (seq_t)1;
        }
        if (factor == 0) return (seq_t)0;
        result *= factor;
        if (m == 1) break;
        limit = int_sqrt_u64(m);
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        uint64_t p = m;
        exp = 1;                              /* exponent is 1 */
        seq_t factor;
        if ((p & 3ULL) == 1ULL) {             /* p ≡ 1 (mod 4) */
            factor = (seq_t)(exp + 1);        /* = 2 */
        } else {                              /* p ≡ 3 (mod 4) */
            factor = (exp & 1ULL) ? (seq_t)0 : (seq_t)1; /* = 0 */
        }
        if (factor == 0) return (seq_t)0;
        result *= factor;
    }

    return result;
}
