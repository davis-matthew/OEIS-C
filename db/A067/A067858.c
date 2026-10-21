/*
 * A067858  J_n(n), where J is the Jordan function, J_n(n) = n^n
 *          product{p|n}(1 - 1/p^n), the product is over the distinct primes,
 *          p, dividing n.
 * https://oeis.org/A067858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = J_n(n) = n^n ∏_{p|n} (1 – 1/p^n) Equivalent integer
 *            implementation: for each prime factor p^e of n multiply the
 *            result by (p^n – 1) and by p^((e‑1)n). This yields a(n) =
 *            ∏_{p|n} p^((e-1)n) · (p^n – 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* static void factor_u64(...) */
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* Undefined for n = 0; return 0 for safety. */
    if (n == 0) return 0;

    /* Factor n.  For n ≤ 10000 at most 20 distinct prime factors. */
    uint64_t primes[20], exps[20];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);   /* static function defined in header */

    seq_t result = 1;
    seq_t seq_n   = (seq_t)n;           /* n as seq_t for exponentiation */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p  = (seq_t)primes[i];
        size_t e = exps[i];

        /* p^n  */
        seq_t p_pow_n = big_pow(p, seq_n);

        /* term = p^n – 1  */
        seq_t term = p_pow_n - 1;
        result *= term;

        /* multiply by p^((e‑1)n) when e > 1  */
        if (e > 1) {
            seq_t exp = (seq_t)((e - 1) * n);
            seq_t p_pow_exp = big_pow(p, exp);
            result *= p_pow_exp;
        }
    }

    return result;
}
