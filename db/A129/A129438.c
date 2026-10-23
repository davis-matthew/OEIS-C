/*
 * A129438  Expansion of (phi(q) * phi(q^2) + phi(-q^2) * phi(q^4)) / 2 in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A129438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is the n-th coefficient of
 *            (phi(q)*phi(q^2)+phi(-q^2)*phi(q^4))/2, where phi is the
 *            Ramanujan theta function
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* SPDX-License-Identifier: MIT */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;          /* work in native 64‑bit for indices   */
    int64_t  res = 0;                  /* running total of the convolution    */

    /* ----   Product 1 : phi(q) * phi(q^2)   ---- */
    for (uint64_t i = 0; i * i <= N; ++i) {
        uint64_t a = i * i;                  /* index from phi(q)                */
        int64_t  ca = (i == 0 ? 1 : 2);      /* coefficient: 1 at 0, else 2     */

        for (uint64_t m = 0; ; ++m) {
            uint64_t b   = 2 * m * m;        /* index from phi(q^2)             */
            uint64_t sum = a + b;
            if (sum > N) break;             /* no further matches for this i   */

            if (sum == N) {
                int64_t cb = (m == 0 ? 1 : 2); /* coefficient from phi(q^2)   */
                res += ca * cb;
            }
        }
    }

    /* ----   Product 2 : phi(-q^2) * phi(q^4)   ---- */
    for (uint64_t m = 0; 2 * m * m <= N; ++m) {
        uint64_t a  = 2 * m * m;                    /* index from phi(-q^2)       */
        int64_t  ca;
        if (m == 0) {
            ca = 1;
        } else {
            /* (-1)^{m^2} is -1 when m is odd, +1 when m is even */
            ca = (m & 1) ? -2 : 2;                  /* coefficient from phi(-q^2) */
        }

        for (uint64_t k = 0; ; ++k) {
            uint64_t b   = 4 * k * k;               /* index from phi(q^4)       */
            uint64_t sum = a + b;
            if (sum > N) break;                     /* no further matches for this m */

            if (sum == N) {
                int64_t cb = (k == 0 ? 1 : 2);      /* coefficient from phi(q^4) */
                res += ca * cb;
            }
        }
    }

    /* The combined coefficient is always even; divide by 2 to get a(n). */
    return (seq_t)(res / 2);
}
