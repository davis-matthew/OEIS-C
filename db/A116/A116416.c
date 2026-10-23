/*
 * A116416  If n = Sum_{m>=1} 2^(m-1) * b(n,m), where each b(n,m) is 0 or 1
 *          and the sum is a finite sum, then a(n) = numerator of Sum_{m>=1}
 *          b(n,m)/m.
 * https://oeis.org/A116416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn, look
 * Formula:   a(n) = numerator of Σ_{m≥1} b(n,m)/m where b(n,m) are the binary
 *            digits of n (1‑based)
 * Range:     u8 n <= 86, u16 n <= 1494, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute a(n) for the sequence A116416. */
seq_t a(idx_t n)
{
    /* No set bits ⇒ numerator 0. */
    if (n == 0) return 0;

    /* At most 64 bits for an unsigned 64‑bit index. */
    uint8_t pos[64];
    size_t cnt = 0;
    idx_t tmp = n;
    idx_t m    = 1;   /* m is the position of the current bit, 1‑based. */

    while (tmp != 0)
    {
        if (tmp & 1)
            pos[cnt++] = (uint8_t)m;
        ++m;
        tmp >>= 1;
    }

    /* Compute lcm of all m values (small, fits in seq_t). */
    seq_t L = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t mval = (seq_t)pos[i];
        seq_t g    = gcd(L, mval);
        L = (L / g) * mval;      /* lcm = (L/g)*mval */
    }

    /* Sum L/m over all m. */
    seq_t N = 0;
    for (size_t i = 0; i < cnt; ++i)
        N += L / (seq_t)pos[i];

    /* Reduce the fraction N/L to lowest terms. */
    seq_t g = gcd(N, L);
    return N / g;
}
