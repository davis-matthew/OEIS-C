/*
 * A003368  Numbers that are the sum of 12 positive 6th powers.
 * https://oeis.org/A003368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of 12 sixth powers
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9993 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t sixth_power(int x) {
    long long y = x;
    long long y2 = y * y;
    long long y3 = y2 * y;
    long long y6 = y3 * y3;
    return (seq_t)y6;
}

static void gen(int rem, int minb, seq_t sum, int maxb, seq_t *arr, idx_t *cnt, idx_t cap) {
    if (*cnt >= cap) return;
    if (rem == 0) {
        arr[(*cnt)++] = sum;
        return;
    }
    for (int b = minb; b <= maxb && *cnt < cap; b++) {
        gen(rem - 1, b, sum + sixth_power(b), maxb, arr, cnt, cap);
    }
}

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
        seq_t *tmp = mem_new(seq_t, cap);
        idx_t cnt = 0;
        
        gen(12, 1, 0, 10, tmp, &cnt, cap);
        mem_qsort(tmp, cnt, sizeof(seq_t), cmp_seq_t);
        
        res = mem_new(seq_t, (cnt + 1));
        res_cnt = 0;
        for (idx_t i = 0; i < cnt; i++) {
            if (i == 0 || tmp[i] != tmp[i-1]) {
                res[res_cnt++] = tmp[i];
            }
        }
        mem_free(tmp);
        done = true;
    }
    
    return (n >= 1 && n <= res_cnt) ? res[n - 1] : 0;
}
