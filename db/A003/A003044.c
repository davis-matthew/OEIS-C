/*
 * A003044  For n > 4, a(n) is the least integer > a(n-1) with precisely two
 *          representations a(n) = a(i) + a(j), 1 <= i < j < n; and a(n) = n
 *          for n=1..4.
 * https://oeis.org/A003044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   greedy search. a(1..4) = 1,2,3,4. Each later a(n) is the least s
 *            > a(n-1) for which exactly two pairs i < j < n have a(i) + a(j)
 *            = s. Pair-sum counts (capped at 3) are kept in a table that is
 *            updated as each term is added
 * Range:     u8 n <= 51, u16 n <= 3579, u32 n <= 5440, u64 n <= 5440,
 *            u128 n <= 5440, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *a3044_v = NULL;
static idx_t a3044_len = 0, a3044_vcap = 0;
static unsigned char *a3044_cnt = NULL;
static size_t a3044_ccap = 0;

static void a3044_ensure(size_t need) {
    size_t nc;
    if (need <= a3044_ccap) return;
    nc = a3044_ccap ? a3044_ccap : 1024;
    while (nc < need) nc *= 2;
    a3044_cnt = (unsigned char *)realloc(a3044_cnt, nc);
    memset(a3044_cnt + a3044_ccap, 0, nc - a3044_ccap);
    a3044_ccap = nc;
}

static void a3044_push(uint64_t x) {
    idx_t i;
    if (a3044_len >= a3044_vcap) {
        a3044_vcap = a3044_vcap ? a3044_vcap * 2 : 256;
        a3044_v = (uint64_t *)realloc(a3044_v, a3044_vcap * sizeof(uint64_t));
    }
    a3044_ensure((size_t)(2 * x + 2));
    for (i = 0; i < a3044_len; i++) {
        uint64_t s = a3044_v[i] + x;
        if (a3044_cnt[s] < 3) a3044_cnt[s]++;
    }
    a3044_v[a3044_len++] = x;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a3044_len == 0) {
        a3044_push(1);
        a3044_push(2);
        a3044_push(3);
        a3044_push(4);
    }
    while (a3044_len < n) {
        uint64_t last = a3044_v[a3044_len - 1];
        uint64_t s = last + 1;
        while (s < a3044_ccap && a3044_cnt[s] != 2) s++;
        a3044_push(s);
    }
    return (seq_t)a3044_v[n - 1];
}
