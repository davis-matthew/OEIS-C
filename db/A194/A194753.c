/*
 * A194753  Number of k such that {-k*e} > {-n*e}, where { } = fractional
 *          part.
 * https://oeis.org/A194753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k | 1 ≤ k < n, {-k*e} > {-n*e}} Since {-x} = 1 - {x}
 *            for non‑integer x, this is equivalent to a(n) = #{k | 1 ≤ k < n,
 *            {k*e} < {n*e}}
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* high‑precision value of e (10 decimal digits beyond the double limit) */
    static const long double E = 2.7182818284590452353602874713527L;
    long double fn = fmodl((long double)n * E, 1.0L);

    seq_t cnt = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        long double fk = fmodl((long double)k * E, 1.0L);
        if (fk < fn)
            ++cnt;
    }
    return cnt;
}
