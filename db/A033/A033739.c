/*
 * A033739  Expansion of Product_{d | 27} theta_3(q^d).
 * https://oeis.org/A033739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A033739 – Expansion of Product_{d | 27} theta_3(q^d)
    Formula: a(n) = # of integer solutions (a,b,c,d) to
               a^2 + 3*b^2 + 9*c^2 + 27*d^2 = n
    (each variable ranges over all integers, positive, negative, or zero.)   */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

/* integer square root for 64‑bit values */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    /* initial guess using double sqrt, then correct */
    uint64_t r = (uint64_t)sqrt((double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

seq_t a(idx_t n)
{
    seq_t ans = 0;
    uint64_t N = (uint64_t)n;   /* working range 0 … n */

    /* iterate over non‑negative a, b, c;
       multiplicity factor m_v = 1 for v==0, 2 for v>0  */
    uint64_t lim_a = isqrt_uint64(N);
    for (uint64_t a = 0; a <= lim_a; ++a) {
        seq_t m_a = (a == 0) ? 1 : 2;
        uint64_t t1 = a * a;
        if (t1 > N) break;
        uint64_t rem1 = N - t1;

        uint64_t lim_b = isqrt_uint64(rem1 / 3);
        for (uint64_t b = 0; b <= lim_b; ++b) {
            seq_t m_b = (b == 0) ? 1 : 2;
            uint64_t t2 = t1 + 3 * b * b;
            if (t2 > N) break;
            uint64_t rem2 = N - t2;

            uint64_t lim_c = isqrt_uint64(rem2 / 9);
            for (uint64_t c = 0; c <= lim_c; ++c) {
                seq_t m_c = (c == 0) ? 1 : 2;
                uint64_t t3 = t2 + 9 * c * c;
                if (t3 > N) break;
                uint64_t rem3 = N - t3;

                if (rem3 % 27 != 0) continue;
                uint64_t d2 = rem3 / 27;
                uint64_t d  = isqrt_uint64(d2);
                if (d * d != d2) continue;   /* not a perfect square */

                seq_t m_d = (d == 0) ? 1 : 2;
                ans += m_a * m_b * m_c * m_d;
            }
        }
    }
    return ans;
}
