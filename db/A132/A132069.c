/*
 * A132069  Expansion of eta(q) * eta(q^2)^2 * eta(q^5)^3 / eta(q^10)^2 in
 *          powers of q.
 * https://oeis.org/A132069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 841, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A132069
 *  Formula: a(n) = -b(n) where
 *    b(5^e) = 1,
 *    b(2^e) = 2 - ((-2)^(e+1)-1)/(-2-1),
 *    b(p^e) = (p^(e+1)-1)/(p-1)  if p ≡ 1 or 9 (mod 10),
 *    b(p^e) = ((-p)^(e+1)-1)/(-p-1)  if p ≡ 3 or 7 (mod 10).
 *    b is multiplicative, so b(n)=∏b(p_i^e_i).  Then a(n) = -b(n).
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    size_t cnt = 0;
    uint64_t primes[64], exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned long e = (unsigned long)exps[i];
        seq_t term;

        if (p == 5) {
            term = 1;
        } else if (p == 2) {
            /* b(2^e) = 2 - ((-2)^(e+1)-1)/(-3) */
            seq_t pow2 = 1;
            seq_t base = -(seq_t)2;
            for (unsigned long j = 0; j < e + 1; ++j) pow2 *= base;
            seq_t numer = pow2 - 1;
            seq_t denom = -3;         /* -2-1 */
            seq_t div = numer / denom;
            term = 2 - div;
        } else {
            unsigned rem = (unsigned)(p % 10);
            if (rem == 1 || rem == 9) {
                /* term = (p^(e+1)-1)/(p-1) */
                seq_t p_pow = 1;
                for (unsigned long j = 0; j < e + 1; ++j) p_pow *= (seq_t)p;
                term = (p_pow - 1) / ((seq_t)p - 1);
            } else /* rem == 3 or rem == 7 */ {
                seq_t p_pow = 1;
                for (unsigned long j = 0; j < e + 1; ++j) p_pow *= (seq_t)p;
                seq_t pow_neg = ((e + 1) & 1) ? -p_pow : p_pow;   /* (-p)^(e+1) */
                seq_t numer = pow_neg - 1;
                seq_t denom = -((seq_t)p + 1);   /* -(p+1) */
                term = numer / denom;
            }
        }

        prod *= term;
    }

    return -prod;
}
