/*
 * A115011  Array read by antidiagonals: let V(m,n) = Sum_{i=1..m, j=1..n,
 *          gcd(i,j)=1} (m+1-i)*(n+1-j), then T(m,n) = 2*(2*m*n+m+n+2*V(m,n)),
 *          for m >= 0, n >= 0.
 * https://oeis.org/A115011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) where (m,n) are the coordinates on the
 *            antidiagonal corresponding to index n. T(m,n) = 2*(2*m*n + m + n
 *            + 2*V(m,n)) V(m,n) = Σ_{i=1..m, j=1..n, gcd(i,j)=1}
 *            (m+1-i)*(n+1-j) The mapping from n to (m,n) follows the standard
 *            “read by antidiagonals” order:
 *            (0,0),(0,1),(1,0),(0,2),(1,1),(...
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_D 200  /* sufficient for n up to ~10^4 (d≈141) */

static seq_t table[MAX_D][MAX_D];
static unsigned char computed[MAX_D][MAX_D] = {0};

static seq_t get_T(seq_t m, seq_t n) {
    unsigned int mi = (unsigned int)m;
    unsigned int ni = (unsigned int)n;

    if (computed[mi][ni]) return table[mi][ni];

    seq_t V = 0;
    for (seq_t i = 1; i <= m; ++i) {
        for (seq_t j = 1; j <= n; ++j) {
            if (gcd(i, j) == 1)
                V += (m + 1 - i) * (n + 1 - j);
        }
    }
    seq_t val = 2 * (2 * m * n + m + n + 2 * V);
    table[mi][ni] = val;
    computed[mi][ni] = 1;
    return val;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    idx_t offset = n - d * (d + 1) / 2;
    idx_t m = offset;
    idx_t n2 = d - offset;

    return get_T(m, n2);
}
