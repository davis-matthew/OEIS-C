/*
 * A006432  Number of tree-rooted planar maps with 3 vertices and n faces and
 *          no isthmuses.
 * https://oeis.org/A006432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*4^n +
 *            binomial(2n,n)*(2n+1)*(n^3-3n^2-10n-12)/(6(n+2)), obtained by
 *            inclusion-exclusion over the isthmus tree edges of Mullin's
 *            shuffle encoding of tree-rooted maps (2 tree edges, n-1 cotree
 *            edges)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   central_binom_a006432, ugcd_a006432
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/central_binom_a006432.h"
#include "../utils/ugcd_a006432.h"

seq_t a(idx_t n)
{
    long long N = (long long)n;
    long long P = N * N * N - 3 * N * N - 10 * N - 12;
    bool neg = P < 0;
    unsigned long long ap = neg ? (unsigned long long)(-P) : (unsigned long long)P;
    unsigned long long d = 6ULL * ((unsigned long long)n + 2);
    unsigned long long o = 2ULL * (unsigned long long)n + 1;
    unsigned long long g;
    seq_t t, T, p;
    idx_t i;

    g = ugcd_a006432(d, ap);
    if (g) {
        d /= g;
        ap /= g;
    }
    g = ugcd_a006432(d, o);
    d /= g;
    o /= g;

    t = central_binom_a006432(n);
    T = t / (seq_t)d;
    T *= (seq_t)o;
    T *= (seq_t)ap;

    p = (seq_t)n + 1;
    for (i = 0; i < n; i++)
        p *= 4;

    return neg ? p - T : p + T;
}
