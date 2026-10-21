/*
 * A209613  Expansion of q * phi(-q^2)^2 * psi(q^3) * psi(-q^3)^2 / psi(q) in
 *          powers of q where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A209613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is the multiplicative function given in the OEIS
 *            description of A209613. For each prime p^e factor: - if p = 2:
 *            a(2^e) = (-1)^e - if p = 3: a(3^e) = (-3)^e - if p > 3: let f =
 *            Kronecker(3, p) (Legendre/Jacobi symbol (3|p)) then a(p^e) =
 *            (-1)^{e*(p mod 12 > 6)} * (p^{e+1} - f^{e+1})/(p - f) T...
 * Range:     i8 n <= 12, i16 n <= 856, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/jacobi_symbol.h"

static seq_t pow_seq(seq_t base, unsigned int exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n >= 1; the empty product gives 1 for n = 1 */
    static const size_t MAX_FACTORS = 32;   /* plenty for any 64‑bit n */
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        unsigned int e = (unsigned int)exps[i];
        seq_t factor;

        if (p == 2)
        {
            /* a(2^e) = (-1)^e */
            factor = (e & 1) ? (seq_t)-1 : (seq_t)1;
        }
        else if (p == 3)
        {
            /* a(3^e) = (-3)^e */
            factor = pow_seq((seq_t)-3, e);
        }
        else
        {
            /* p > 3 */
            int t = jacobi_symbol(3, p);   /* (3|p) = ±1 */
            seq_t f = (seq_t)t;           /* f = ±1 */

            /* p^{e+1} */
            seq_t p_pow = pow_seq((seq_t)p, e + 1);

            /* f^{e+1} : only ±1, compute directly */
            seq_t f_pow = ((e + 1) & 1) ? f : (seq_t)1;

            seq_t numerator   = p_pow - f_pow;
            seq_t denominator = (seq_t)p - f;
            factor = numerator / denominator;      /* division exact */

            /* extra sign if p mod 12 > 6 */
            if ((p % 12) > 6 && (e & 1))
                factor = -factor;
        }

        result *= factor;
    }

    return result;
}
