/*
 * A011703  A binary m-sequence: expansion of reciprocal of
 *          x^8+x^7+x^6+x^4+x^3+x^2+1.
 * https://oeis.org/A011703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-2) ⊕ a(n-3) ⊕ a(n-4) ⊕ a(n-6) ⊕ a(n-7) ⊕ a(n-8)),
 *            with a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0, a(7)=1. The sequence
 *            is 85‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for one full period (0..84) */
    static seq_t period[85] = {0};
    static int built = 0;                /* 0: not yet computed, 1: ready */

    if (!built) {
        /* initialise first 8 terms */
        period[0] = period[1] = period[2] = period[3] = period[4] =
        period[5] = period[6] = 0;
        period[7] = 1;

        /* compute remaining terms of the period (8..84) */
        for (idx_t i = 8; i <= 84; ++i) {
            seq_t v = 0;
            v ^= period[i - 2];
            v ^= period[i - 3];
            v ^= period[i - 4];
            v ^= period[i - 6];
            v ^= period[i - 7];
            v ^= period[i - 8];
            period[i] = v;
        }
        built = 1;
    }

    /* return value modulo 85 */
    return period[n % 85];
}
