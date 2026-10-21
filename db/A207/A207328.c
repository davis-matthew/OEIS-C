/*
 * A207328  Nonlinearities of 4-variable Boolean functions ordered
 *          lexicographically.
 * https://oeis.org/A207328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{u∈GF(2)^4, b∈{0,1}} HammingDistance( f , a_{u,b} )
 *            where f is the Boolean function whose truth‑table is the 16‑bit
 *            integer n, and a_{u,b}(x) = u·x ⊕ b is an affine Boolean
 *            function
 * Range:     u8 n <= 9388, u16 n <= 9388, u32 n <= 9388, u64 n <= 9388,
 *            u128 n <= 9388, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The affine functions are precomputed once. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 32 affine functions: mask[(u<<1)|b] holds the 16‑bit truth table. */
    static bool init = false;
    static uint16_t mask[32];
    if (!init)
    {
        for (uint16_t u = 0; u < 16; ++u)
        {
            for (uint16_t b = 0; b < 2; ++b)
            {
                uint16_t m = 0;
                for (uint16_t x = 0; x < 16; ++x)
                {
                    int val = ((__builtin_popcount(u & x) & 1) ^ b);
                    if (val) m |= (1u << x);
                }
                mask[(u << 1) | b] = m;
            }
        }
        init = true;
    }

    uint16_t f = (uint16_t)n;            /* function truth table */
    uint16_t min_dist = 16;              /* worst case distance */

    for (int i = 0; i < 32; ++i)
    {
        uint16_t diff = f ^ mask[i];
        uint16_t d    = (uint16_t)__builtin_popcount((unsigned int)diff);
        if (d < min_dist) min_dist = d;
    }

    return (seq_t)min_dist;
}
