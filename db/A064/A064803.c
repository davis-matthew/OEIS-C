/*
 * A064803  Number of subgroups of the group C_n X C_n X C_n (where C_n is the
 *          cyclic group of order n).
 * https://oeis.org/A064803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} \sum_{j=0}^{2e}
 *            (e‑⌊(j‑1)/2⌋)(2j‑⌊(j‑1)/2⌋)·p^(2e‑j)
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 3359, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(p) = 2(p²+p+2) for prime p.  The function is multiplicative. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorize_uint64.h"

/* fast exponentiation for seq_t */
static seq_t pow_uint(uint64_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* a(p^e) for a single prime power */
static seq_t prime_term(uint64_t p, int e)
{
    idx_t exp2 = (idx_t)e * 2;                    /* 2e */
    seq_t term = (seq_t)0;
    seq_t power = pow_uint(p, exp2);              /* p^(2e) */

    for (idx_t j = 0; j <= exp2; ++j) {
        int64_t f;
        if (j == 0) f = -1;                     /* floor((j-1)/2) for j=0 */
        else f = (int64_t)((j - 1) / 2);        /* truncates toward zero */

        int64_t a = (int64_t)e - f;
        int64_t b = 2 * (int64_t)j - f;
        seq_t coeff = (seq_t)(a * b);
        term += coeff * power;

        if (j < exp2) power /= (seq_t)p;
    }
    return term;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for offset 1 */

    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    size_t factor_cnt;

    factorize_uint64(m, primes, exps, &factor_cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < factor_cnt; ++i) {
        seq_t term = prime_term(primes[i], exps[i]);
        result *= term;
    }

    return result;
}
