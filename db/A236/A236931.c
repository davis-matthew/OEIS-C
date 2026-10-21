/*
 * A236931  Number of integer solutions to a^2 + b^2 + c^2 + 6*d^2 = n.
 * https://oeis.org/A236931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 8074, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = number of integer solutions to a^2 + b^2 + c^2 + 6*d^2 = n */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values (floor(sqrt(x))) */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1u << 62);            /* largest 4^k <= 2^64 */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Loop over non‑negative d; account for signs later */
    idx_t max_d = isqrt_u64(n / 6);
    for (idx_t d = 0; d <= max_d; ++d) {
        seq_t d2 = (seq_t)6 * (seq_t)d * (seq_t)d;
        seq_t rem_after_d = (seq_t)n - d2;
        idx_t max_a = isqrt_u64((idx_t)rem_after_d);

        for (idx_t a = 0; a <= max_a; ++a) {
            seq_t a2 = (seq_t)a * (seq_t)a;
            seq_t rem_after_a = rem_after_d - a2;
            idx_t max_b = isqrt_u64((idx_t)rem_after_a);

            for (idx_t b = 0; b <= max_b; ++b) {
                seq_t b2 = (seq_t)b * (seq_t)b;
                seq_t rem_after_b = rem_after_a - b2;
                idx_t c = isqrt_u64((idx_t)rem_after_b);

                if ((seq_t)c * (seq_t)c == rem_after_b) {
                    seq_t factor = 1;
                    if (d > 0) factor *= 2;
                    if (a > 0) factor *= 2;
                    if (b > 0) factor *= 2;
                    if (c > 0) factor *= 2;
                    count += factor;
                }
            }
        }
    }

    return count;
}
