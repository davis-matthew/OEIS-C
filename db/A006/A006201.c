/*
 * A006201  Number of colorings of labeled graphs on n nodes using exactly 3
 *          colors, divided by 48.
 * https://oeis.org/A006201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{s=1..n-2} C(n-1,s-1) * 2^(s*(n-s)-2) * V(n-s), where
 *            V(m) = Sum_{j=1..m-1} C(m-1,j-1) * 2^(j*(m-j)-1). (Equivalent to
 *            n!*2^C(n,2)*[x^n] (E(x)-1)^3 / 48: unordered 3-block set
 *            partitions, block of node 1 has size s, rest split into 2
 *            unordered blocks, each cross-block pair is an option...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t p2(idx_t e) {
    seq_t r = 1, b = 2;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;

    seq_t *row = alloc_zero(n + 1);
    seq_t *V = alloc_zero(n + 1);
    row[0] = 1; /* row 0 of Pascal's triangle */

    for (idx_t r = 0; r + 2 <= n; r++) {
        /* row holds C(r, .) */
        idx_t m = r + 1;
        if (m >= 2) {
            seq_t s = 0;
            for (idx_t j = 1; j < m; j++)
                s += row[j - 1] * p2(j * (m - j) - 1);
            V[m] = s;
        }
        /* advance to row r+1 */
        for (idx_t k = r + 1; k >= 1; k--)
            row[k] += row[k - 1];
    }
    /* row now holds C(n-1, .) */
    seq_t result = 0;
    for (idx_t s = 1; s + 2 <= n; s++)
        result += row[s - 1] * p2(s * (n - s) - 2) * V[n - s];

    release(row, n + 1);
    release(V, n + 1);
    return result;
}
