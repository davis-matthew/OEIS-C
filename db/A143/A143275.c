/*
 * A143275  A054525 * A029935.
 * https://oeis.org/A143275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏ₚ a(pᵉ) where a(p) = 2·p – 3, a(pᵉ) = (e·(p‑1) + p +
 *            2)·(p‑1)²·p^(e‑3) for e > 1
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined for n = 0, just guard */
        return 0;

    idx_t  m      = n;      /* number to be factorised */
    seq_t  result = 1;      /* multiplicative accumulator */

    /* trial division – no external helper required */
    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p != 0)          /* p is not a factor of m */
            continue;

        /* determine exponent e of prime p in m */
        int e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        /* compute the contribution of prime power pᵉ */
        seq_t term;
        if (e == 1)
        {
            term = (seq_t)2 * (seq_t)p - 3;
        }
        else if (e == 2)
        {
            /* (2·(p‑1)+p+2)·(p‑1)² / p */
            seq_t num = ((seq_t)2 * (seq_t)(p - 1) + (seq_t)p + 2) *
                         ((seq_t)(p - 1) * (seq_t)(p - 1));
            term = num / (seq_t)p;
        }
        else /* e ≥ 3 */
        {
            seq_t mult1 = (seq_t)e * (seq_t)(p - 1) + (seq_t)p + 2;
            seq_t mult2 = (seq_t)(p - 1) * (seq_t)(p - 1);
            seq_t pow   = 1;
            for (int i = 0; i < e - 3; ++i)
                pow *= (seq_t)p;
            term = mult1 * mult2 * pow;
        }

        result *= term;
    }

    /* remaining prime factor larger than √n (if any) */
    if (m > 1)
    {
        seq_t term = (seq_t)2 * (seq_t)m - 3;
        result *= term;
    }

    return result;
}
