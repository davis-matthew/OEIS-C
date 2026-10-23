/*
 * A131947  Expansion of (1 - (phi(-q) * phi(-q^3))^2)/4 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A131947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is multiplicative with a(2^e) = 3 - 2^(e+1), a(3^e) = 1,
 *            a(p^e) = (p^(e+1)-1)/(p-1) for p>3. (Implement by
 *            prime‑factorising n.)
 * Range:     i8 n <= 12, i16 n <= 445, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    if (n <= 1) return (n == 1) ? (seq_t)1 : (seq_t)0;  /* a(1) = 1, a(0) unused */

    uint64_t nn = (uint64_t)n;
    /* array sizes are sufficient for any 64‑bit integer (max 16 distinct primes) */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        idx_t    e  = (idx_t)exps[i];
        seq_t   a_pe;

        if (p == 2) {
            seq_t pow2 = pow_int((seq_t)2, e + 1);
            a_pe = (seq_t)3 - pow2;
        } else if (p == 3) {
            a_pe = (seq_t)1;
        } else {
            seq_t numerator = pow_int((seq_t)p, e + 1) - (seq_t)1;
            seq_t denom     = (seq_t)(p - 1);
            a_pe = numerator / denom;
        }
        result *= a_pe;
    }
    return result;
}
