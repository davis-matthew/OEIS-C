/*
 * A132003  Expansion of (phi(q^3) / phi(q)) * phi(-q^2) * phi(-q^6) in powers
 *          of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A132003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*b(n) where b is multiplicative: * b(2^e) = -1 * b(3^e)
 *            = 1 * b(p^e) = e+1 if p ≡ 1 (mod 4) * b(p^e) = (1+(-1)^e)/2 if p
 *            ≡ 3 (mod 4) * a(0) = 1, a(1) = -2
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)-2;

    uint64_t nn = (uint64_t)n;
    /* The number of distinct prime factors of a 64‑bit integer is at most 64,
       so a buffer of 128 is more than sufficient. */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t b = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t f;
        if (p == 2) {
            f = (seq_t)-1;                     /* b(2^e) = -1 */
        } else if (p == 3) {
            f = (seq_t)1;                      /* b(3^e) = 1 */
        } else if ((p & 3ULL) == 1ULL) {       /* p ≡ 1 (mod 4) */
            f = (seq_t)(e + 1);
        } else {                               /* p ≡ 3 (mod 4) */
            f = ((e & 1ULL) == 0) ? (seq_t)1 : (seq_t)0;
        }
        b *= f;
    }

    return (seq_t)(-2 * b);
}
