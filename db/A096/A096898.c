/*
 * A096898  Least area/6 of primitive Pythagorean triangles with even leg 4n.
 * https://oeis.org/A096898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ n*(m²‑k²)/3 : m>k>0, gcd(m,k)=1, (m mod 2)!=(k mod
 *            2), m*k=2n } // (area/6) (This is equivalent to mn*(m²‑k²)/6
 *            with mn = 2n.)
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd64.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;               /* input index */
    unsigned __int128 target = (unsigned __int128)N * 2ULL; /* 2N = m*k */

    seq_t best = 0;                         /* 0 means unset */

    for (uint64_t d = 1; (unsigned __int128)d * d <= target; ++d) {
        if ((unsigned __int128)target % d != 0)
            continue;

        uint64_t d2 = (uint64_t)(target / d);

        uint64_t m = d > d2 ? d : d2;       /* larger factor */
        uint64_t k = d < d2 ? d : d2;       /* smaller factor */

        if (m == k)                 /* not primitive (m=n) */
            continue;
        if (gcd64(m, k) != 1)       /* not coprime */
            continue;
        if ((m & 1ULL) == (k & 1ULL)) /* same parity */
            continue;

        seq_t mm = (seq_t)m;
        seq_t kk = (seq_t)k;

        seq_t diff = mm * mm - kk * kk;        /* m²‑k² */
        seq_t cand = (seq_t)N * diff / (seq_t)3; /* n*(m²‑k²)/3 */

        if (best == 0 || cand < best)
            best = cand;
    }

    return best;
}
