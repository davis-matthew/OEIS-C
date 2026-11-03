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
 * Validated: standard, pass, 78 terms through n = 9618 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

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
