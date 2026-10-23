/*
 * A170968  Expansion of Product_{i=0..m-1} (1 + x^(4*i+1)) for m = 5.
 * https://oeis.org/A170968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in product_{i=0..4} (1 + x^(4*i+1))
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* maximal exponent from (1 + x^1)(1 + x^5)(1 + x^9)(1 + x^13)(1 + x^17) */
    const idx_t MAXN = 45u;
    static seq_t coeff[46];
    static bool ready = false;

    if (!ready)
    {
        for (size_t i = 0; i < sizeof(coeff) / sizeof(coeff[0]); ++i)
            coeff[i] = 0;

        coeff[0] = 1;
        const uint8_t exps[5] = {1, 5, 9, 13, 17};

        for (size_t ei = 0; ei < 5; ++ei)
        {
            idx_t e = exps[ei];
            for (int i = (int)MAXN; i >= (int)e; --i)
                coeff[i] = coeff[i] + coeff[i - e];
        }
        ready = true;
    }

    if (n > MAXN)
        return (seq_t)0;
    return coeff[(size_t)n];
}
