/*
 * A131999  Expansion of eta(q)^2 * eta(q^2) * eta(q^4)^3 / eta(q^8)^2 in
 *          powers of q.
 * https://oeis.org/A131999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*b(n) where b is multiplicative: b(2^e) = 1 For odd
 *            prime p: if p ≡ 1 or 7 (mod 8): b(p^e) = (p^(e+1) – 1)/(p – 1)
 *            if p ≡ 3 or 5 (mod 8): b(p^e) = ((–p)^(e+1) – 1)/(–p – 1)
 *            Initial values: a(0)=1, a(1)=‑2, a(2)=‑2, a(3)=4, …
 * Range:     i8 n <= 12, i16 n <= 793, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(0) is the offset term */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* strip factors of 2 – a(2 n) = a(n) */
    idx_t m = n;
    while ((m & 1ULL) == 0)
        m >>= 1;

    /* powers of two <=> odd part 1 */
    if (m == 1)
        return (seq_t)(-2);

    /* factor the odd part */
    const size_t MAXP = 64;
    uint64_t primes[MAXP];
    uint64_t exps[MAXP];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* multiplicative product for b */
    seq_t b = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* the case p == 2 should not occur after stripping */
        if (p == 2) {
            continue;
        }

        seq_t term;

        if (p % 8 == 1 || p % 8 == 7) {
            /* b(p^e) = (p^(e+1)-1)/(p-1)  */
            seq_t powp = seq_pow((seq_t)p, e + 1);
            seq_t numer = powp - (seq_t)1;
            seq_t denom = (seq_t)(p - 1);
            term = numer / denom;
        } else { /* p % 8 == 3 or 5 */
            /* b(p^e) = ((-p)^(e+1)-1)/(-p-1)  */
            seq_t powp = seq_pow((seq_t)p, e + 1);

            seq_t numer;
            /* (-p)^(e+1) = (-1)^(e+1) * p^(e+1)  */
            if (((e + 1) & 1) == 1)        /* odd exponent of -p */
                numer = -powp - (seq_t)1;
            else
                numer = powp - (seq_t)1;

            seq_t denom = -(seq_t)(p + 1);
            term = numer / denom;
        }

        b = seq_mul(b, term);
    }

    /* a(n) = -2 * b */
    return seq_mul(b, (seq_t)(-2));
}
