/*
 * A088534  Number of representations of n by the quadratic form x^2 + xy +
 *          y^2 with 0 <= x <= y.
 * https://oeis.org/A088534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) counts the number of integer pairs (x,y) with 0 ≤ x ≤ y
 * such that n = x^2 + x*y + y^2.
 * Algorithm: The representation can be rewritten as
 *
 *   4n = (2x + y)^2 + 3y^2,
 *
 * so for each y with 3y² ≤ 4n we compute rhs = 4n – 3y².
 * If rhs is a perfect square s², then x = (s – y)/2 is integer and
 * 0 ≤ x ≤ y.  Count such solutions.
 *
 * Integer square roots are computed by a shift‑by‑two algorithm that
 * uses only addition, subtraction, and shifts – thus it works for
 * any seq_t width (including 128‑bit) without overflow.
 *
 * Complexity:  O(√n) per call, well below the 2‑second limit for n ≤ 10000.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t isqrt_seq(seq_t x)
{
    seq_t res = 0;
    /* largest power of 4 ≤ x */
    seq_t bit = ((seq_t)1 << (((sizeof(seq_t) * 8) - 2) & ~1));
    while (bit > x)
        bit >>= 2;
    while (bit)
    {
        if (x >= res + bit)
        {
            x -= res + bit;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    /* iterate y while 3y² ≤ 4n */
    for (idx_t y = 0;
         (seq_t)3 * y * y <= (seq_t)4 * n;
         ++y)
    {
        seq_t rhs = (seq_t)4 * n - (seq_t)3 * y * y;
        seq_t s   = isqrt_seq(rhs);
        if (s * s != rhs)                /* not a perfect square   */
            continue;
        if (s < y)                       /* would give negative x   */
            continue;
        if ((s - y) & 1)                 /* x not integer          */
            continue;
        seq_t x = (s - y) / 2;
        if (x <= y)                      /* 0 ≤ x ≤ y satisfied   */
            ++cnt;
    }
    return cnt;
}
