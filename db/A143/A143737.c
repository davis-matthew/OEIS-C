/*
 * A143737  Aliquot sequence starting at 68.
 * https://oeis.org/A143737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 68, a(n+1) = σ(a(n)) – a(n) where σ(m) is the sum of all
 *            divisors of m
 * Range:     u8 n <= 0, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void sigma_uint64(uint64_t n, seq_t *sigma_out)
{
    if (n == 0)
    {
        *sigma_out = (seq_t)0;
        return;
    }

    /* factor_u64 fills pr and ex with the prime factors and exponents.
       The number of distinct prime factors of a 64‑bit integer is at most 16. */
    uint64_t pr[16], ex[16];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t sum = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)pr[i];
        seq_t p_pow = (seq_t)1;
        for (uint64_t k = 0; k < ex[i] + 1; ++k)
            p_pow *= p;                     /* p^(e+1) */

        /* (p^(e+1) – 1) / (p – 1) is an integer. */
        seq_t term = (p_pow - (seq_t)1) / (p - (seq_t)1);
        sum *= term;                      /* multiplicative property */
    }
    *sigma_out = sum;
}

seq_t a(idx_t n)
{
    seq_t val = (seq_t)68;
    for (idx_t i = 0; i < n; ++i)
    {
        if (val == (seq_t)0)
            break;

        seq_t sum;
        sigma_uint64((uint64_t)val, &sum);
        val = sum - val; /* aliquot sum */
    }
    return val;
}
