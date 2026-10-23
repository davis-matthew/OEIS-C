/*
 * A129303  Expansion of eta(q^2)^3 * eta(q^5)^2 * eta(q^10) / eta(q)^2 in
 *          powers of q.
 * https://oeis.org/A129303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of a(p^e) where
 *            - a(p^e) = p^e if p = 2 or 5; - a(p^e) = (p^(e+1) - 1)/(p – 1)
 *            if p ≡ 1 or 9 (mod 10); - a(p^e) = (p^(e+1) + (-1)^e)/(p + 1) if
 *            p ≡ 3 or 7 (mod 10)
 * Range:     u8 n <= 16, u16 n <= 418, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute base^exp for small exponents.  The exponent is small enough
   that seq_t arithmetic suffices. */
static inline seq_t pow_seq(seq_t base, seq_t exp)
{
    seq_t result = 1;
    while (exp != 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (idx_t)n;   /* a(1)=1, a(0)=0 (offset guard) */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        seq_t term;

        if (p == 2 || p == 5)
        {
            term = pow_seq(p, e);
        }
        else if ((p % 10 == 1) || (p % 10 == 9))
        {
            seq_t pow_next = pow_seq(p, e + 1);
            term = (pow_next - 1) / (p - 1);
        }
        else /* p ≡ 3 or 7 (mod 10) */
        {
            seq_t pow_next = pow_seq(p, e + 1);
            if (e & 1)
                term = (pow_next - 1) / (p + 1);   /* e odd */
            else
                term = (pow_next + 1) / (p + 1);   /* e even */
        }
        result *= term;
    }
    return result;
}
