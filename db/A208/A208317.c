/*
 * A208317  Number of n X 3 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5, a(2)=113, a(3)=2852, a(4)=71956. For n>4:
 *            a(n)=24*a(n-1)+32*a(n-2)-24*a(n-3). The function memoises the
 *            recurrence and keeps at most 10000 terms in static storage. For
 *            n>10000 it continues the recurrence using just the last three
 *            values
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t cache[10000]; /* cache for up to 10000 terms */
static size_t len = 0;     /* number of terms currently stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* harmless guard */

    /* initialise cache on first call */
    if (len == 0) {
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)113;
        cache[2] = (seq_t)2852;
        cache[3] = (seq_t)71956;
        len     = 4;
    }

    if (n <= (idx_t)10000) {             /* compute inside cache */
        if ((size_t)n > len) {
            for (idx_t i = (idx_t)len + 1; i <= n; ++i) {
                size_t idx = (size_t)(i - 1); /* zero‑based */
                cache[idx] =
                    (seq_t)24 * cache[idx - 1] +
                    (seq_t)32 * cache[idx - 2] -
                    (seq_t)24 * cache[idx - 3];
            }
            len = (size_t)n;
        }
        return cache[(size_t)(n - 1)];
    }

    /* n > 10000: continue recurrence beyond cache limit */
    if (len < 10000) {
        for (idx_t i = (idx_t)len + 1; i <= (idx_t)10000; ++i) {
            size_t idx = (size_t)(i - 1);
            cache[idx] =
                (seq_t)24 * cache[idx - 1] +
                (seq_t)32 * cache[idx - 2] -
                (seq_t)24 * cache[idx - 3];
        }
        len = 10000;
    }

    /* start from the last three stored terms */
    seq_t a1 = cache[9997]; /* a(9998) */
    seq_t a2 = cache[9998]; /* a(9999) */
    seq_t a3 = cache[9999]; /* a(10000) */

    for (idx_t i = 10001; i <= n; ++i) {
        seq_t next = (seq_t)24 * a3 + (seq_t)32 * a2 - (seq_t)24 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
