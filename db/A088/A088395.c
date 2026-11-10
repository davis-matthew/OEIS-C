/*
 * A088395  a(n) = A069525(n)/n.
 * https://oeis.org/A088395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A069525(n)/n, where A069525(n) is the smallest multiple
 *            of n with digit sum 6 (0 if none); computed by BFS over
 *            (remainder, digit sum) states, then long division of the digit
 *            string by n
 * Range:     u8 n <= 72, u16 n <= 132, u32 n <= 832, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 95 terms through n = 917 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t N = (size_t)n;
    size_t total = 7 * N;
    int *parent = (int *)malloc(total * sizeof(int));
    unsigned char *digit = (unsigned char *)malloc(total);
    int *queue = (int *)malloc(total * sizeof(int));
    if (!parent || !digit || !queue) {
        free(parent); free(digit); free(queue);
        return 0;
    }
    for (size_t i = 0; i < total; ++i) parent[i] = -1;

    size_t head = 0, tail = 0;
    for (int d = 1; d <= 6; ++d) {
        size_t rem = (size_t)d % N;
        size_t idx = (size_t)d * N + rem;
        if (parent[idx] != -1) continue;
        parent[idx] = -2;
        digit[idx] = (unsigned char)d;
        queue[tail++] = (int)idx;
    }

    while (head < tail) {
        int curIdx = queue[head++];
        size_t sum = (size_t)curIdx / N;
        size_t rem = (size_t)curIdx % N;
        if (sum == 6 && rem == 0) {
            unsigned char *rev = (unsigned char *)malloc(total);
            if (!rev) { free(parent); free(digit); free(queue); return 0; }
            size_t len = 0;
            int idx = curIdx;
            while (parent[idx] != -2) {
                rev[len++] = digit[idx];
                idx = parent[idx];
            }
            rev[len++] = digit[idx];

            uint64_t r = 0;
            seq_t q = 0;
            for (size_t i = 0; i < len; ++i) {
                uint64_t cur = r * 10 + rev[len - 1 - i];
                q = q * 10 + (seq_t)(cur / (uint64_t)n);
                r = cur % (uint64_t)n;
            }
            free(rev); free(parent); free(digit); free(queue);
            return q;
        }
        for (int d = 0; d <= 6; ++d) {
            size_t ns = sum + (size_t)d;
            if (ns > 6) break;
            size_t nr = (rem * 10 + (size_t)d) % N;
            size_t ni = ns * N + nr;
            if (parent[ni] != -1) continue;
            parent[ni] = curIdx;
            digit[ni] = (unsigned char)d;
            queue[tail++] = (int)ni;
        }
    }
    free(parent); free(digit); free(queue);
    return 0;
}
