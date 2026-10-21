/*
 * A081812  If n = p_1^e_1 * ... * p_k^e_k, p_1 < ... < p_k primes, then a(n)
 *          = Max{ Max(p_i, e_i) }.
 * https://oeis.org/A081812
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ p_i, e_i } for the prime-factorisation n = Π p_i^e_i
 *            (offset 2)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n should be ≥ 2 for the test set; return 0 for smaller n (unused). */
    if (n < 2) return 0;

    /* Storage for at most 64 prime factors (ample for 64‑bit n). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factorise n into primes and exponents. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t max_val = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if ((seq_t)primes[i] > max_val) max_val = (seq_t)primes[i];
        if ((seq_t)exps[i]   > max_val) max_val = (seq_t)exps[i];
    }
    return max_val;
}
