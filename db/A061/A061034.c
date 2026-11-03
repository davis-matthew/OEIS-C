/*
 * A061034  Maximal number of subgroups in an Abelian group with n elements.
 * https://oeis.org/A061034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 255, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9958 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061034: Maximal number of subgroups in an Abelian group of order n.
   Formula: a(n) = ∏_{p^e‖n} Σ_{k=0}^e [e choose k]_p
   where [e choose k]_p is the Gaussian binomial coefficient over GF(p). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute Σ_{k=0}^e [e choose k]_p for the elementary abelian group (C_p)^e. */
static seq_t subspace_count(seq_t p, idx_t e)
{
    enum { MAX_E = 64 };        /* sufficient for any uint64_t exponent */
    seq_t pow_p[MAX_E + 1];
    pow_p[0] = (seq_t)1;
    for (idx_t i = 1; i <= e; ++i)
        pow_p[i] = pow_p[i - 1] * p;   /* p^i */

    seq_t total = (seq_t)1;          /* k = 0 contributes 1 */
    seq_t binom = (seq_t)1;          /* [e choose 0]_p = 1 */

    for (idx_t k = 1; k <= e; ++k) {
        seq_t num = pow_p[e - k + 1] - (seq_t)1; /* p^{e-k+1} - 1 */
        seq_t den = pow_p[k] - (seq_t)1;          /* p^k - 1 */
        binom = binom * num / den;                /* integer division */
        total += binom;
    }
    return total;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Factorise n.  The helper fills primes[], exps[], and the count. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];
        result = result * subspace_count(p, e);
    }
    return result;
}
