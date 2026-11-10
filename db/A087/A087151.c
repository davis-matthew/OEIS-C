/*
 * A087151  Positive square-root of terms of the self-convolution of A087150.
 * https://oeis.org/A087151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = sqrt(Sum_{k=1..n} A087150(k)*A087150(n-k+1)); computed by
 *            search: A087150(1)=1, and A087150(n) is the least integer >
 *            A087150(n-1) such that 2*A087150(n) + Sum_{k=2..n-1}
 *            A087150(k)*A087150(n-k+1) is a perfect square s^2, with a(n)=s
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 40, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A087151_isqrt_floor(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (hi * hi <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    static seq_t *B = NULL;   /* A087150 terms, 1-based */
    static seq_t *S = NULL;   /* A087151 terms, 1-based */
    static size_t cap = 0;
    static size_t done = 0;

    if (n == 0)
        return 0;

    size_t need = (size_t)n + 1;
    if (need > cap)
    {
        size_t nc = cap ? cap : 16;
        while (nc < need)
            nc <<= 1;
        B = mem_resize(seq_t, B, nc);
        S = mem_resize(seq_t, S, nc);
        cap = nc;
    }

    for (size_t i = done + 1; i <= (size_t)n; ++i)
    {
        if (i == 1)
        {
            B[1] = 1;
            S[1] = 1;
            continue;
        }
        seq_t t = 0;
        for (size_t k = 2; k + 1 <= i; ++k)
            t += B[k] * B[i - k + 1];

        seq_t prev = B[i - 1];
        seq_t x = t + prev * 2 + 2;     /* need s^2 >= x */
        seq_t s = A087151_isqrt_floor(x);
        if (s * s < x)
            ++s;
        /* s^2 - t must be even: s and t same parity */
        if (((s ^ t) & 1) != 0)
            ++s;
        B[i] = (s * s - t) / 2;
        S[i] = s;
    }
    if ((size_t)n > done)
        done = (size_t)n;
    return S[n];
}
