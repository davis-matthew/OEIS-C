/*
 * A226977  Places n where A225867(n) <= 2.
 * https://oeis.org/A226977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th m >= 6 with A225867(m) <= 2, where A225867(m) =
 *            max_{2<=k<=(m-2)/2} #{d>1: d|m+k, k|m+d}; found by a search with
 *            early exit, cached
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* returns 1 if A225867(m) <= 2 */
static int A226977_small(idx_t m)
{
    idx_t max_k = (m - 2) / 2;
    for (idx_t k = 2; k <= max_k; ++k) {
        idx_t t = m + k;
        int count = 0;
        for (idx_t d = 1; d * d <= t; ++d) {
            if (t % d == 0) {
                idx_t d1 = d, d2 = t / d;
                if (d1 > 1 && (m + d1) % k == 0) ++count;
                if (d2 != d1 && d2 > 1 && (m + d2) % k == 0) ++count;
                if (count > 2) return 0;
            }
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    static idx_t terms[64];
    static int nterms = 0;
    static idx_t next = 6;
    const idx_t LIMIT = 1000; /* last known term is 839; none more up to 1e9 */

    if (n < 1 || n > 60) return 0;
    while ((idx_t)nterms < n && next <= LIMIT) {
        if (A226977_small(next)) terms[nterms++] = next;
        ++next;
    }
    if ((idx_t)nterms >= n) return (seq_t)terms[n - 1];
    return 0;
}
