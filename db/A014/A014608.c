/*
 * A014608  a(n) = (4n)!/(24^n).
 * https://oeis.org/A014608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n)!/(24^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* use 64‑bit temporaries for the intermediate product */
        unsigned long long four_k = 4ull * k;
        unsigned long long term = (four_k - 3ull) * (four_k - 2ull);
        term *= (four_k - 1ull);
        term *= four_k;
        term /= 24ull;            /* divisible exactly */
        res *= (seq_t)term;
    }
    return res;
}
