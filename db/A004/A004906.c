/*
 * A004906  Numbers that are the sum of at most 11 nonzero 10th powers.
 * https://oeis.org/A004906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most 11 nonzero 10th powers
 *            with bases 1..k, keep those below (k+1)^10 (which are then
 *            complete), sort, dedupe, and raise k until n terms exist
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   A004906_cmp, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/A004906_cmp.h"

static uint64_t *A004906_buf = NULL;
static size_t A004906_len = 0, A004906_cap = 0;
static uint64_t A004906_pw[64];

static void A004906_push(uint64_t v) {
    if (A004906_len == A004906_cap) {
        A004906_cap = A004906_cap ? A004906_cap * 2 : 1024;
        A004906_buf = (uint64_t *)realloc(A004906_buf, A004906_cap * sizeof(uint64_t));
    }
    A004906_buf[A004906_len++] = v;
}

static void A004906_dfs(int start, int k, int left, uint64_t sum, uint64_t limit) {
    A004906_push(sum);
    if (left == 0) return;
    for (int b = start; b <= k; b++) {
        uint64_t s = sum + A004906_pw[b];
        if (s >= limit) break;
        A004906_dfs(b, k, left - 1, s, limit);
    }
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t count = 0;
    static int k = 0;

    if (A004906_pw[1] == 0) {
        for (int b = 0; b < 64; b++) {
            uint64_t p = 1;
            for (int i = 0; i < 10; i++) p *= (uint64_t)b;
            A004906_pw[b] = p;
        }
    }

    while ((idx_t)count < n && k < 60) {
        k++;
        uint64_t limit = A004906_pw[k + 1];
        A004906_len = 0;
        A004906_dfs(1, k, 11, 0, limit);
        qsort(A004906_buf, A004906_len, sizeof(uint64_t), A004906_cmp);
        size_t u = 0;
        for (size_t i = 0; i < A004906_len; i++) {
            if (u == 0 || A004906_buf[i] != A004906_buf[u - 1])
                A004906_buf[u++] = A004906_buf[i];
        }
        free(terms);
        terms = (uint64_t *)malloc((u ? u : 1) * sizeof(uint64_t));
        for (size_t i = 0; i < u; i++) terms[i] = A004906_buf[i];
        count = u;
    }

    return u128_to_seq((unsigned __int128)terms[n - 1]);
}
