/*
 * A004288  Least positive multiple of n written in base 8 using only 0 and 1.
 * https://oeis.org/A004288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   breadth-first search over residues mod n for the least number
 *            with base-8 digits only 0 and 1 that is divisible by n; the
 *            base-8 digit string is returned read as a decimal number
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t r0 = 1 % m;
    if (r0 == 0) return (seq_t)1;

    int64_t *parent = (int64_t *)malloc(sizeof(int64_t) * m);
    unsigned char *digit = (unsigned char *)malloc(m);
    unsigned char *seen = (unsigned char *)calloc(m, 1);
    uint64_t *queue = (uint64_t *)malloc(sizeof(uint64_t) * m);
    uint64_t head = 0, tail = 0;

    seen[r0] = 1;
    parent[r0] = -1;
    digit[r0] = 1;
    queue[tail++] = r0;

    int found = 0;
    while (head < tail && !found) {
        uint64_t r = queue[head++];
        for (int d = 0; d <= 1; d++) {
            uint64_t s = (r * 8 + (uint64_t)d) % m;
            if (!seen[s]) {
                seen[s] = 1;
                parent[s] = (int64_t)r;
                digit[s] = (unsigned char)d;
                queue[tail++] = s;
                if (s == 0) { found = 1; break; }
            }
        }
    }

    /* reconstruct digits from residue 0 back to the root */
    uint64_t len = 0;
    int64_t cur = 0;
    while (cur != -1) {
        len++;
        cur = parent[cur];
    }
    unsigned char *digs = (unsigned char *)malloc(len);
    uint64_t i = len;
    cur = 0;
    while (cur != -1) {
        digs[--i] = digit[cur];
        cur = parent[cur];
    }

    seq_t val = 0;
    for (i = 0; i < len; i++) {
        val = val * 10 + (seq_t)digs[i];
    }

    free(digs);
    free(parent);
    free(digit);
    free(seen);
    free(queue);
    return val;
}
