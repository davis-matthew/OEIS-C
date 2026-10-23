/*
 * A036752  Number of strings of n distinct digits from 1-9 that are the last
 *          n digits of a square in base 8.
 * https://oeis.org/A036752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = 720 for n ≥ 6, otherwise computed by brute force. The
 *            value equals the number of n‑digit strings of distinct digits
 *            from 1–7 that are the last n octal digits of a perfect square
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n >= 6) return (seq_t)720;          /* constant for n ≥ 6 */

    idx_t m = (idx_t)1;
    for (idx_t i = 0; i < n; ++i) m *= (idx_t)8;   /* m = 8^n */

    unsigned char *res = (unsigned char *)calloc((size_t)m, sizeof(unsigned char));
    if (!res) return (seq_t)0;            /* allocation failure */

    for (idx_t s = 0; s < m; ++s) {
        idx_t sq = (s * s) % m;
        res[sq] = 1;
    }

    idx_t cnt = 0;
    for (idx_t x = 0; x < m; ++x) {
        if (!res[x]) continue;
        idx_t t = x;
        unsigned mask = 0;
        bool ok = true;
        for (idx_t i = 0; i < n; ++i) {
            unsigned d = (unsigned)(t & 7u);          /* octal digit */
            if (d == 0u) { ok = false; break; }
            unsigned bit = 1u << d;
            if (mask & bit) { ok = false; break; }
            mask |= bit;
            t >>= 3u;
        }
        if (ok) ++cnt;
    }

    free(res);
    return (seq_t)cnt;
}
