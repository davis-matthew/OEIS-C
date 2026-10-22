/*
 * A003368  Numbers that are the sum of 12 positive 6th powers.
 * https://oeis.org/A003368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of 12 sixth powers
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gen, sixth_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gen.h"
#include "../utils/sixth_power.h"

static int cmp_seq_t(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n) {
    static bool done = false;
    static seq_t *res = NULL;
    static idx_t res_cnt = 0;
    
    if (!done) {
        idx_t cap = 500000;
        seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
        idx_t cnt = 0;
        
        gen(12, 1, 0, 10, tmp, &cnt, cap);
        qsort(tmp, cnt, sizeof(seq_t), cmp_seq_t);
        
        res = (seq_t *)malloc((cnt + 1) * sizeof(seq_t));
        res_cnt = 0;
        for (idx_t i = 0; i < cnt; i++) {
            if (i == 0 || tmp[i] != tmp[i-1]) {
                res[res_cnt++] = tmp[i];
            }
        }
        free(tmp);
        done = true;
    }
    
    return (n >= 1 && n <= res_cnt) ? res[n - 1] : 0;
}
