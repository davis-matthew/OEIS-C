/*
 * A024846  a(n) = least m such that if r and s in {1/1, 1/2, 1/3, ..., 1/n}
 *          satisfy r < s, then r < k/m < (k+4)/m < s for some integer k.
 * https://oeis.org/A024846
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least integer m such that for every j with 2 <= j <=
 *            n there exists an integer k satisfying (k+4)*(j-1) < m < k*j .
 *            The algorithm searches for the minimal m using the inequality
 *            floor(m/j)+1 <= (m - 4*j + 3)/(j-1)
 * Range:     u8 n <= 8, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* value for indices below the offset is irrelevant */

    idx_t j;
    seq_t m = 0;

    /* Minimum m for a single j is m_j = 4*j*(j-1)+2*j-1 */
    for (j = 2; j <= n; ++j) {
        __int128 val = (__int128)4 * j * (j - 1) + (__int128)2 * j - 1;
        seq_t candidate = (seq_t)val;
        if (candidate > m) m = candidate;
    }

    /* Incrementally search for the smallest m that satisfies all j. */
    while (1) {
        bool ok = true;

        for (j = 2; j <= n; ++j) {
            /* l = floor(m/j) + 1  (smallest integer > m/j) */
            seq_t l = m / j + 1;

            /* u = floor((m - 4*j + 3) / (j-1))  (largest integer < m/(j-1)-4) */
            __int128 tmp = (__int128)m - (__int128)4 * j + 3;
            if (tmp < 0) {      /* m too small for this j (should not happen) */
                seq_t m_j = (seq_t)((__int128)4 * j * (j - 1) + (__int128)2 * j - 1);
                m = m_j;
                ok = false;
                break;
            }
            seq_t u = (seq_t)(tmp / (j - 1));

            if (l > u) {        /* failure for this j, skip to the next allowed interval */
                seq_t k = l;    /* k is floor(m/j)+1, the smallest k that could work */
                __int128 new_m128 = (__int128)(k + 4) * (j - 1) + 1;
                seq_t new_m = (seq_t)new_m128;
                if (new_m <= m) new_m = m + 1;
                m = new_m;
                ok = false;
                break;          /* restart check with the updated m */
            }
        }

        if (ok) return m;      /* all conditions satisfied */
    }
}
