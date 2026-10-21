/*
 * A229342  a(n) = sigma(n'), the sum of divisors of the arithmetic derivative
 *          of n.
 * https://oeis.org/A229342
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n')
 * Range:     u8 n <= 63, u16 n <= 5183, u32 n <= 8734, u64 n <= 8734,
 *            u128 n <= 8734, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"

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
