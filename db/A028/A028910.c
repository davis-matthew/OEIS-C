/*
 * A028910  Arrange digits of 2^n in descending order.
 * https://oeis.org/A028910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = descending_digits(2^n)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 3166 (2026-11-01)
 * Helpers:   digit_frequencies, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_frequencies.h"
#include "../utils/pow2.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    seq_t v = pow2(n);
    unsigned int freq[10] = {0};
    digit_frequencies(v, freq);

    seq_t res = 0;
    for (int d = 9; d >= 0; d--)
    {
        unsigned int c = freq[d];
        for (unsigned int i = 0; i < c; i++)
        {
            res = mul_small(res, 10);
            res += d;
        }
    }
    return res;
}
