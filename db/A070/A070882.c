/*
 * A070882  Least k such that A070867(k) = n, or 0 if no such k exists.
 * https://oeis.org/A070882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k such that A070867(k) = n, or 0 if none; A070867:
 *            a(1)=a(2)=1, a(n)=a(n-1-a(n-1))+a(n-1-a(n-2)). Computed by a
 *            search: generate A070867 up to k = 8n+100 and take the first k
 *            hitting n (0 if none found)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    idx_t limit = 8 * n + 100;
    idx_t *c = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!c)
        exit(1);

    c[0] = 0;
    c[1] = 1;
    c[2] = 1;
    idx_t result = 0;

    if (n == 1) {
        free(c);
        return (seq_t)1;
    }

    for (idx_t i = 3; i <= limit; ++i) {
        idx_t i1 = i - 1 - c[i - 1];
        idx_t i2 = i - 1 - c[i - 2];
        c[i] = c[i1] + c[i2];
        if (c[i] == n) {
            result = i;
            break;
        }
    }

    free(c);
    return (seq_t)result;
}
