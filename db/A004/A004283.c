/*
 * A004283  Least positive multiple of n written in base 3 using only 0 and 1.
 * https://oeis.org/A004283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: breadth-first search over residues mod n of base-3
 *            numbers with digits 0/1 (appending 0 before 1 gives the least
 *            multiple); the base-3 digits are then read as a decimal number
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 244, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    uint64_t m = (uint64_t)n;
    long long *parent = (long long *)malloc(m * sizeof(long long));
    unsigned char *digit = (unsigned char *)malloc(m);
    unsigned char *seen = (unsigned char *)calloc(m, 1);
    uint64_t *queue = (uint64_t *)malloc(m * sizeof(uint64_t));
    uint64_t head = 0, tail = 0;
    uint64_t start = 1 % m;
    seen[start] = 1;
    parent[start] = -1;
    digit[start] = 1;
    queue[tail++] = start;
    uint64_t found = start;
    int done = (start == 0);
    while (!done && head < tail) {
        uint64_t r = queue[head++];
        for (unsigned d = 0; d <= 1; d++) {
            uint64_t s = (r * 3 + d) % m;
            if (!seen[s]) {
                seen[s] = 1;
                parent[s] = (long long)r;
                digit[s] = (unsigned char)d;
                queue[tail++] = s;
                if (s == 0) { found = s; done = 1; break; }
            }
        }
    }
    /* reconstruct digits (reverse order) */
    unsigned char *digs = (unsigned char *)malloc(m + 1);
    uint64_t len = 0;
    long long cur = (long long)found;
    while (cur != -1) {
        digs[len++] = digit[cur];
        cur = parent[cur];
    }
    seq_t val = 0;
    for (uint64_t i = len; i > 0; i--) {
        val = val * 10 + (seq_t)digs[i - 1];
    }
    free(digs);
    free(parent);
    free(digit);
    free(seen);
    free(queue);
    return val;
}
