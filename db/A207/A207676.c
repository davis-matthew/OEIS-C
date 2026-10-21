/*
 * A207676  Nonlinearities of 3-variable Boolean functions ordered
 *          lexicographically.
 * https://oeis.org/A207676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 4 – max_{u=0..7} |∑_{x=0..7} (-1)^(f(x)⊕(u·x))| / 2 where
 *            f(x) is the x‑th bit of the 8‑bit truth table n (LSB = x=0)
 * Range:     u8 n <= 255, u16 n <= 255, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Only the lowest 8 bits of n describe a 3‑variable Boolean function. */
    const unsigned int truth = (unsigned int)(n & 0xFF);

    int max_abs_w = 0;  /* maximum absolute Walsh value over all u. */

    for (int u = 0; u < 8; ++u) {
        int w = 0;    /* Walsh sum for this u. */

        for (int x = 0; x < 8; ++x) {
            int f   = (truth >> x) & 1;           /* f(x) */
            int prod = ((x & u) ^
                        ((x & u) >> 1) ^
                        ((x & u) >> 2)) & 1;     /* u·x mod 2 (dot product) */

            /* Contribution = +1 if f⊕dot == 0, else -1. */
            w += (f ^ prod) ? -1 : 1;
        }

        if (w < 0) w = -w;          /* absolute value */
        if (w > max_abs_w) max_abs_w = w;
    }

    /* Nonlinearity: 4 – (max_abs_w / 2).  max_abs_w is always even. */
    int nl = 4 - (max_abs_w / 2);
    return (seq_t)nl;
}
