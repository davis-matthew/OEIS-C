/*
 * A004911  Numbers that are the sum of at most 5 positive 11th powers.
 * https://oeis.org/A004911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all multisets of at most 5 positive 11th
 *            powers, keep the sums below (B+1)^11, sort and remove duplicates
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 728, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

#define MAXB 40

static uint64_t *vals = NULL;
static idx_t vals_len = 0;
static uint64_t p11[MAXB + 1];
static uint64_t limit;

static int cmp_u64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static void gen(int minb, uint64_t sum, int cnt, uint64_t *arr, idx_t *pos) {
    if (sum < limit) arr[(*pos)++] = sum;
    if (cnt >= 5) return;
    for (int b = minb; b <= MAXB; b++) {
        uint64_t s = sum + p11[b];
        if (s >= limit) break;
        gen(b, s, cnt + 1, arr, pos);
    }
}

static void init(void) {
    if (vals) return;
    for (int b = 0; b <= MAXB; b++) {
        uint64_t r = 1;
        for (int i = 0; i < 11; i++) r *= (uint64_t)b;
        p11[b] = r;
    }
    {
        uint64_t r = 1;
        for (int i = 0; i < 11; i++) r *= (uint64_t)(MAXB + 1);
        limit = r;
    }
    /* number of multisets of size <= 5 from MAXB items = C(MAXB+5, 5) */
    idx_t cap = 1;
    for (int i = 1; i <= 5; i++) cap = cap * (idx_t)(MAXB + i) / (idx_t)i;
    uint64_t *arr = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    idx_t pos = 0;
    gen(1, 0, 0, arr, &pos);
    qsort(arr, (size_t)pos, sizeof(uint64_t), cmp_u64);
    idx_t j = 0;
    for (idx_t i = 0; i < pos; i++)
        if (j == 0 || arr[i] != arr[j - 1]) arr[j++] = arr[i];
    vals = arr;
    vals_len = j;
}

seq_t a(idx_t n) {
    init();
    if (n < 1 || n > vals_len) return (seq_t)0;
    return u128_to_seq((unsigned __int128)vals[n - 1]);
}
