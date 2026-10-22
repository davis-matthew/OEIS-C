/*
 * A004912  Numbers that are the sum of at most 6 positive 11th powers.
 * https://oeis.org/A004912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the n-th number expressible as a sum of at most 6
 *            positive 11th powers
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 28, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static void gen(seq_t *pow, int pc, int pi, seq_t sum, int c, seq_t *sums, idx_t *cnt) {
    if (pi == pc) {
        sums[(*cnt)++] = sum;
        return;
    }
    for (int i = 0; i <= c; i++) {
        gen(pow, pc, pi + 1, sum + (seq_t)i * pow[pi], c - i, sums, cnt);
    }
}

static int cmp(const void *a, const void *b) {
    seq_t x = *(seq_t *)a, y = *(seq_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t csz = 0;
    
    if (cache && n <= csz) return cache[n - 1];
    
    seq_t pow[20];
    for (int k = 0; k < 20; k++) {
        seq_t p = 1;
        for (int i = 0; i < 11; i++) p *= (seq_t)(k + 1);
        pow[k] = p;
    }
    
    seq_t *sums = (seq_t *)malloc(300000 * sizeof(seq_t));
    idx_t cnt = 0;
    gen(pow, 20, 0, 0, 6, sums, &cnt);
    
    qsort(sums, cnt, sizeof(seq_t), cmp);
    
    idx_t uc = 0;
    for (idx_t i = 0; i < cnt; i++) {
        if (i == 0 || sums[i] != sums[i - 1]) {
            sums[uc++] = sums[i];
        }
    }
    
    cache = sums;
    csz = uc;
    
    return (n <= csz) ? cache[n - 1] : 0;
}
