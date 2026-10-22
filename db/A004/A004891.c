/*
 * A004891  Numbers that are the sum of at most 7 positive 9th powers.
 * https://oeis.org/A004891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve/search: enumerate all sums of at most 7 ninth powers b^9
 *            with 1<=b<=B, sort, deduplicate; terms < (B+1)^9 are complete,
 *            increase B until n terms are complete
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128_t;

static u128_t *cache = NULL;
static idx_t cache_complete = 0;
static idx_t cache_B = 0;

static u128_t *work = NULL;
static idx_t work_cnt = 0;
static u128_t pw[64];

static int cmp_u128(const void *x, const void *y) {
    u128_t u = *(const u128_t *)x, v = *(const u128_t *)y;
    if (u < v) return -1;
    if (u > v) return 1;
    return 0;
}

static u128_t pow9(idx_t b) {
    u128_t r = 1;
    int i;
    for (i = 0; i < 9; i++) r *= (u128_t)b;
    return r;
}

static void gen(idx_t start, int depth, u128_t sum, idx_t B) {
    idx_t i;
    work[work_cnt++] = sum;
    if (depth == 7) return;
    for (i = start; i < B; i++) gen(i, depth + 1, sum + pw[i], B);
}

static void build(idx_t B) {
    /* number of multisets of size <= 7 from B items = C(B+7,7) */
    u128_t c = 1;
    idx_t i, uniq, comp;
    u128_t lim;
    for (i = 1; i <= 7; i++) c = c * (u128_t)(B + i) / (u128_t)i;
    for (i = 0; i < B; i++) pw[i] = pow9(i + 1);
    work = (u128_t *)malloc((size_t)c * sizeof(u128_t));
    work_cnt = 0;
    gen(0, 0, 0, B);
    qsort(work, (size_t)work_cnt, sizeof(u128_t), cmp_u128);
    uniq = 0;
    for (i = 0; i < work_cnt; i++)
        if (i == 0 || work[i] != work[uniq - 1]) work[uniq++] = work[i];
    lim = pow9(B + 1);
    comp = 0;
    while (comp < uniq && work[comp] < lim) comp++;
    if (cache) free(cache);
    cache = work;
    work = NULL;
    cache_complete = comp;
    cache_B = B;
}

seq_t a(idx_t n) {
    idx_t B = cache_B;
    if (B == 0) B = 1;
    while (cache == NULL || cache_complete < n) {
        build(B);
        B++;
    }
    return u128_to_seq(cache[n - 1]);
}
