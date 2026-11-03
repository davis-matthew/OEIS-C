/*
 * A229342  a(n) = sigma(n'), the sum of divisors of the arithmetic derivative
 *          of n.
 * https://oeis.org/A229342
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n')
 * Range:     u8 n <= 63, u16 n <= 5183, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 84 terms through n = 8734 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Compute a(n) = sigma of the arithmetic derivative of n.
 * The arithmetic derivative is n' = Σ e_i * (n / p_i) over
 * the prime factorization n = Π p_i^e_i.  Then a(n) = σ(n'). */
seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 2 */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    /* Factor n */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Compute the arithmetic derivative n' = Σ e_i * (n / p_i) */
    seq_t nprime = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p  = pr[i];
        uint64_t e  = ex[i];
        seq_t term  = (seq_t)e * ((seq_t)n / (seq_t)p);
        nprime += term;
    }

    /* Sum of divisors of n' */
    return sigma_of((idx_t)nprime);
}
