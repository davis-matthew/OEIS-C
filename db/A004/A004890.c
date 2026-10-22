/*
 * A004890  Numbers that are the sum of at most 6 positive 9th powers.
 * https://oeis.org/A004890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most 6 positive 9th powers
 *            k^9 (1 <= k <= B) that are <= (B+1)^9 - 1, sort, remove
 *            duplicates; enlarge B until n terms are known
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   A004890_cmp, A004890_push, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"
#include "../utils/A004890_cmp.h"
#include "../utils/A004890_push.h"

static uint64_t *A004890_vals = NULL;
static size_t A004890_cnt = 0;
static int A004890_B = 0;
static uint64_t A004890_pw[160];

static void A004890_rec(int minb, int B, int rem, uint64_t s, uint64_t L,
                        uint64_t **buf, size_t *len, size_t *cap) {
    int b;
    A004890_push(s, buf, len, cap);
    if (rem == 0) return;
    for (b = minb; b <= B; b++) {
        uint64_t t = s + A004890_pw[b];
        if (t > L) break;
        A004890_rec(b, B, rem - 1, t, L, buf, len, cap);
    }
}

static void A004890_build(int B) {
    int k, j;
    uint64_t L;
    uint64_t *buf = NULL;
    size_t len = 0, cap = 0, i, u;
    for (k = 1; k <= B + 1; k++) {
        uint64_t p = 1;
        for (j = 0; j < 9; j++) p *= (uint64_t)k;
        A004890_pw[k] = p;
    }
    L = A004890_pw[B + 1] - 1;
    A004890_rec(1, B, 6, 0, L, &buf, &len, &cap);
    qsort(buf, len, sizeof(uint64_t), A004890_cmp);
    u = 0;
    for (i = 0; i < len; i++) {
        if (i == 0 || buf[i] != buf[i - 1]) buf[u++] = buf[i];
    }
    free(A004890_vals);
    A004890_vals = buf;
    A004890_cnt = u;
    A004890_B = B;
}

seq_t a(idx_t n) {
    int B = (A004890_B == 0) ? 16 : A004890_B;
    if (A004890_B == 0) A004890_build(B);
    while ((idx_t)A004890_cnt < n && B < 130) {
        B += 8;
        if (B > 130) B = 130;
        A004890_build(B);
    }
    return u128_to_seq((unsigned __int128)A004890_vals[n - 1]);
}
