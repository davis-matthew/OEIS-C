/*
 * A123665  a(n) = Sum_{k=1..21} n^A001358(k).
 * https://oeis.org/A123665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = 1 + Σ_{k ∈
 *            {4,6,9,10,14,15,21,22,25,26,33,34,35,38,39,46,49,51,55,57,58}}
 *            n^k
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* pre‑computed exponents for the sum (taken from A001358) */
    static const uint8_t exps[21] = {
        4, 6, 9, 10, 14, 15, 21, 22, 25, 26,
        33, 34, 35, 38, 39, 46, 49, 51, 55, 57, 58
    };

    seq_t result = 1;
    seq_t base   = (seq_t)n;

    for (size_t i = 0; i < 21; ++i)
    {
        idx_t e = exps[i];
        seq_t term = 1;
        seq_t b = base;

        while (e)
        {
            if (e & 1)
                term *= b;
            b *= b;
            e >>= 1;
        }

        result += term;
    }

    return result;
}
