/*
 * A005588  Number of free binary trees admitting height n.
 * https://oeis.org/A005588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = Sum_{d=n..2n} (T(d) - U(n,d)), where T(d) counts free
 *            trees with max degree <= 3 and diameter d, and U(n,d) counts
 *            those trees in which every vertex at distance n - ceil(d/2) from
 *            the center (or central edge) has degree 3, so that no vertex
 *            with eccentricity n can be a binary root. Both co...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   alloc_zero, m2, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/m2.h"

/* multisets of size 3 from b items: C(b+2,3) */
static seq_t c3(seq_t b)
{
    seq_t t = m2(b);
    if ((b + 2) % 3 == 0) return t * ((b + 2) / 3);
    return t / 3 * (b + 2);
}

static seq_t *Nt;          /* Nt[h+1] = N(h) */

static seq_t Nh(long long h)
{
    if (h < 0) return (seq_t)0;
    return Nt[h + 1];
}

/* rooted binary trees of height <= h in which every vertex at depth k has 2 children */
static seq_t G(long long k, long long h)
{
    long long h0 = h - k, j;
    seq_t v;
    if (h0 <= 0) v = (seq_t)0;
    else v = m2(Nh(h0 - 1));
    for (j = 1; j <= k; j++) {
        long long hh = h0 + j;
        if (hh < 0) v = (seq_t)0;
        else v = m2(v + 1);
    }
    return v;
}

seq_t a(idx_t n)
{
    long long nn = (long long)n, d, h;
    seq_t sum = 0;
    Nt = alloc_zero((idx_t)(nn + 2));
    Nt[0] = 0;
    for (h = 0; h <= nn; h++) {
        seq_t p = Nt[h];
        Nt[h + 1] = m2(p) + p + 1;
    }
    for (d = nn; d <= 2 * nn; d++) {
        seq_t tot, bad = 0;
        if (d % 2 == 1) {
            long long r = (d + 1) / 2, k = nn - r;
            seq_t b = Nh(r - 1) - Nh(r - 2);
            tot = m2(b);
            if (k <= r - 2) {
                seq_t g = G(k, r - 1) - G(k, r - 2);
                bad = m2(g);
            }
        } else {
            long long r = d / 2, k = nn - r;
            seq_t b = Nh(r - 1) - Nh(r - 2);
            seq_t A = Nh(r - 2) + 1;
            tot = m2(b) * A + c3(b);
            if (k <= r - 1) {
                if (k == 0) {
                    bad = m2(b) * Nh(r - 2) + c3(b);
                } else {
                    seq_t lo = G(k - 1, r - 2);
                    seq_t B = G(k - 1, r - 1) - lo;
                    bad = m2(B) * (lo + 1) + c3(B);
                }
            }
        }
        sum += tot - bad;
    }
    release(Nt, (idx_t)(nn + 2));
    Nt = 0;
    return sum;
}
