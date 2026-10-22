/*
 * A004899  Numbers that are the sum of at most 4 nonzero 10th powers.
 * https://oeis.org/A004899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums p^10+q^10+r^10+s^10 with 0 <= p <=
 *            q <= r <= s <= B, keep those below (B+1)^10 (complete there),
 *            sort and dedupe; enlarge B as needed
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t *tab = NULL;
static idx_t tab_len = 0;
static unsigned tab_B = 0;

static void build(unsigned B) {
    uint64_t pw[80];
    unsigned i, j, k, l;
    uint64_t T = 1;
    for (i = 0; i <= B; i++) {
        uint64_t p = 1;
        for (j = 0; j < 10; j++) p *= i;
        pw[i] = p;
    }
    for (j = 0; j < 10; j++) T *= (uint64_t)(B + 1);

    size_t cap = 1024, cnt = 0;
    uint64_t *buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 0; i <= B; i++) {
        uint64_t s1 = pw[i];
        if (s1 >= T) break;
        for (j = i; j <= B; j++) {
            uint64_t s2 = s1 + pw[j];
            if (s2 >= T) break;
            for (k = j; k <= B; k++) {
                uint64_t s3 = s2 + pw[k];
                if (s3 >= T) break;
                for (l = k; l <= B; l++) {
                    uint64_t s4 = s3 + pw[l];
                    if (s4 >= T) break;
                    if (cnt == cap) {
                        cap *= 2;
                        buf = (uint64_t *)realloc(buf, cap * sizeof(uint64_t));
                    }
                    buf[cnt++] = s4;
                }
            }
        }
    }
    qsort(buf, cnt, sizeof(uint64_t), cmp_u64);
    size_t u = 0;
    for (i = 0; i < cnt; i++)
        if (u == 0 || buf[i] != buf[u - 1]) buf[u++] = buf[i];

    free(tab);
    tab = buf;
    tab_len = (idx_t)u;
    tab_B = B;
}

seq_t a(idx_t n) {
    if (tab == NULL) build(16);
    while (tab_len < n && tab_B < 78) {
        unsigned nb = tab_B * 2;
        if (nb > 78) nb = 78;
        build(nb);
    }
    if (n < 1 || n > tab_len) return (seq_t)0;
    return (seq_t)tab[n - 1];
}
