/*
 * A185549  a(n) = ceiling(n^(3/2)); complement of A185550.
 * https://oeis.org/A185549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(n^(3/2))
 * Range:     u8 n <= 40, u16 n <= 1625, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = ⌈n^(3/2)⌉ for any idx_t n >= 0.  */
/* Uses integer arithmetic only; no floating-point or math.h.  */
/* For seq_t types of size ≤ 8 bytes, a 128‑bit intermediate is used.  */
/* For larger seq_t (arbitrary‑precision), the provided floor_sqrt helper is used.  */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static inline unsigned long long sqrt128(unsigned __int128 x)
{
    unsigned long long r   = 0;
    unsigned long long bit = 1ULL << 63;           /* highest bit of 64‑bit result */

    while (bit) {
        unsigned long long cand = r | bit;
        if ((unsigned __int128)cand * cand <= x)
            r = cand;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Small seq_t: use 128‑bit intermediate and integer sqrt. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 cube = (unsigned __int128)n * n * n;
        unsigned long long r  = sqrt128(cube - 1);   /* floor sqrt(cube‑1) */
        return (seq_t)(r + 1);                     /* ceil sqrt(cube) */
    }

    /* Arbitrary‑precision seq_t: use helper floor_sqrt. */
    seq_t x = (seq_t)n;
    x = x * x * (seq_t)n;        /* n³ in seq_t */
    seq_t r = floor_sqrt(x);
    if (r * r == x) return r;    /* perfect square */
    return r + 1;                /* otherwise ceil sqrt */
}
