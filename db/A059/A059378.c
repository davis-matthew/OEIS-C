/*
 * A059378  Jordan function J_5(n).
 * https://oeis.org/A059378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   * a(n) = n^5 * ∏_{p|n} (1 – 1/p^5) * (equivalently: a(p^e) =
 *            p^(5e) – p^(5(e‑1)) and a is multiplicative) * * The following
 *            implementation factors each n by trial division, * computes the
 *            prime‑power contributions by binary exponentiation, * and
 *            multiplies them together. It works for any size of...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_pow_seq_t.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                                 /* undefined for 0 but safe */

    uint64_t m = n;
    seq_t ans  = 1;

    /* Trial division */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            uint64_t exp1 = 5ULL * e;
            uint64_t exp2 = 5ULL * (e - 1);
            seq_t term = mul_pow_seq_t((seq_t)p, exp1) - mul_pow_seq_t((seq_t)p, exp2);
            ans *= term;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1)
    {
        /* e == 1  →  a(p) = p^5 – 1 */
        seq_t term = mul_pow_seq_t((seq_t)m, 5ULL) - 1;
        ans *= term;
    }

    return ans;
}
