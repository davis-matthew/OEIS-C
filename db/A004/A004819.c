/*
 * A004819  Numbers that are the sum of 8 positive 11th powers.
 * https://oeis.org/A004819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 2288, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a4819_cmp, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate all multisets of 8 bases in 1..16, keep the distinct sums of their 11th powers below 17^11 (that range is complete), sort them; a(n) is the n-th smallest */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a4819_cmp.h"

#define A4819_B 16

static uint64_t a4819_pw[A4819_B + 1];
static uint64_t a4819_limit;
static uint64_t *a4819_vals = NULL;
static size_t a4819_cnt = 0, a4819_cap = 0;

static void a4819_rec(int k, int minb, uint64_t s) {
    if (k == 8) {
        if (a4819_cnt == a4819_cap) {
            a4819_cap = a4819_cap ? a4819_cap * 2 : 1024;
            a4819_vals = (uint64_t *)realloc(a4819_vals, a4819_cap * sizeof(uint64_t));
        }
        a4819_vals[a4819_cnt++] = s;
        return;
    }
    for (int b = minb; b <= A4819_B; b++) {
        /* remaining 8-k terms all at least b^11 */
        uint64_t t = s + (uint64_t)(8 - k) * a4819_pw[b];
        if (t >= a4819_limit) break;
        a4819_rec(k + 1, b, s + a4819_pw[b]);
    }
}

static void a4819_init(void) {
    for (int b = 1; b <= A4819_B + 1; b++) {
        uint64_t p = 1;
        for (int e = 0; e < 11; e++) p *= (uint64_t)b;
        if (b <= A4819_B) a4819_pw[b] = p;
        else a4819_limit = p;
    }
    a4819_rec(0, 1, 0);
    qsort(a4819_vals, a4819_cnt, sizeof(uint64_t), a4819_cmp);
    size_t u = 0;
    for (size_t i = 0; i < a4819_cnt; i++)
        if (u == 0 || a4819_vals[i] != a4819_vals[u - 1])
            a4819_vals[u++] = a4819_vals[i];
    a4819_cnt = u;
}

seq_t a(idx_t n) {
    if (a4819_vals == NULL) a4819_init();
    if (n < 1 || n > (idx_t)a4819_cnt) return (seq_t)0;
    return u128_to_seq((unsigned __int128)a4819_vals[n - 1]);
}
