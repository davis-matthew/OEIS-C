/*
 * A088399  a(n)=A069529(n)/n.
 * https://oeis.org/A088399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A069529(n)/n, where A069529(n) is the least multiple of n
 *            with digit sum 10 (0 if none); computed by BFS over (remainder,
 *            digit sum), then long division of the digit string by n
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 98 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if ((n % 3) == 0) return 0;

    const int SM = 10;
    const size_t W = SM + 1;
    const size_t N = (size_t)n * W;

    int *parent = (int *)malloc(N * sizeof(int));
    int *digit = (int *)malloc(N * sizeof(int));
    unsigned char *seen = (unsigned char *)malloc(N);
    int *queue = (int *)malloc(N * sizeof(int));
    if (!parent || !digit || !seen || !queue) {
        free(parent); free(digit); free(seen); free(queue);
        return 0;
    }
    for (size_t i = 0; i < N; ++i) { parent[i] = -1; seen[i] = 0; digit[i] = 0; }

    size_t qh = 0, qt = 0;
    long long found = -1;
    long long nn = (long long)n;

    for (int d = 1; d <= 9 && found < 0; ++d) {
        long long rem = d % nn;
        size_t id = (size_t)rem * W + (size_t)d;
        if (!seen[id]) {
            seen[id] = 1;
            digit[id] = d;
            parent[id] = -1;
            queue[qt++] = (int)id;
            if (rem == 0 && d == SM) found = (long long)id;
        }
    }

    while (found < 0 && qh < qt) {
        size_t cur = (size_t)queue[qh++];
        long long cr = (long long)(cur / W);
        int cs = (int)(cur % W);
        for (int d = 0; d <= 9 && found < 0; ++d) {
            int ns = cs + d;
            if (ns > SM) break;
            long long nr = (cr * 10 + d) % nn;
            size_t id = (size_t)nr * W + (size_t)ns;
            if (!seen[id]) {
                seen[id] = 1;
                parent[id] = (int)cur;
                digit[id] = d;
                queue[qt++] = (int)id;
                if (nr == 0 && ns == SM) found = (long long)id;
            }
        }
    }

    seq_t result = 0;
    if (found >= 0) {
        size_t len = 0;
        int s = (int)found;
        while (s != -1) { ++len; s = parent[s]; }
        int *ds = (int *)malloc(len * sizeof(int));
        if (ds) {
            size_t k = len;
            s = (int)found;
            while (s != -1) { ds[--k] = digit[s]; s = parent[s]; }
            long long r = 0;
            seq_t q = 0;
            for (size_t i = 0; i < len; ++i) {
                r = r * 10 + ds[i];
                q = q * 10 + (seq_t)(r / nn);
                r %= nn;
            }
            result = q;
            free(ds);
        }
    }

    free(parent); free(digit); free(seen); free(queue);
    return result;
}
