/*
 * A004808  Numbers that are the sum of 8 positive 10th powers.
 * https://oeis.org/A004808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: enumerate all nondecreasing 8-tuples of positive bases
 *            with sum of 10th powers <= B, sort and dedupe; double B until at
 *            least n terms
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 7602, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A4808_cmp, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/A4808_cmp.h"

static uint64_t *A4808_pw = NULL;
static int A4808_npw = 0;
static uint64_t *A4808_buf = NULL;
static size_t A4808_cnt = 0, A4808_cap = 0;

static void A4808_push(uint64_t v) {
    if (A4808_cnt == A4808_cap) {
        A4808_cap = A4808_cap ? A4808_cap * 2 : 1024;
        A4808_buf = (uint64_t *)realloc(A4808_buf, A4808_cap * sizeof(uint64_t));
    }
    A4808_buf[A4808_cnt++] = v;
}

static void A4808_dfs(int left, int minb, uint64_t partial, uint64_t B) {
    int b;
    if (left == 0) {
        A4808_push(partial);
        return;
    }
    for (b = minb; b < A4808_npw; b++) {
        uint64_t p = A4808_pw[b];
        if (partial + (uint64_t)left * p > B) break;
        A4808_dfs(left - 1, b, partial + p, B);
    }
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t nterms = 0;
    static uint64_t bound = 0;
    uint64_t B;
    int b;

    if (terms && n <= nterms) return u128_to_seq((unsigned __int128)terms[n - 1]);

    B = bound ? bound * 2 : 16384;
    for (;;) {
        size_t i, u;
        /* bases 1.. with b^10 <= B */
        free(A4808_pw);
        A4808_pw = (uint64_t *)malloc(sizeof(uint64_t) * 2);
        A4808_npw = 1;
        A4808_pw[0] = 0;
        for (b = 1;; b++) {
            unsigned __int128 p = 1;
            int k;
            for (k = 0; k < 10; k++) p *= (unsigned __int128)b;
            if (p > (unsigned __int128)B) break;
            A4808_pw = (uint64_t *)realloc(A4808_pw, sizeof(uint64_t) * (size_t)(b + 1));
            A4808_pw[b] = (uint64_t)p;
            A4808_npw = b + 1;
        }
        A4808_cnt = 0;
        A4808_dfs(8, 1, 0, B);
        qsort(A4808_buf, A4808_cnt, sizeof(uint64_t), A4808_cmp);
        u = 0;
        for (i = 0; i < A4808_cnt; i++)
            if (u == 0 || A4808_buf[i] != A4808_buf[u - 1]) A4808_buf[u++] = A4808_buf[i];
        free(terms);
        terms = (uint64_t *)malloc(sizeof(uint64_t) * (u ? u : 1));
        for (i = 0; i < u; i++) terms[i] = A4808_buf[i];
        nterms = u;
        bound = B;
        if (n <= nterms) return u128_to_seq((unsigned __int128)terms[n - 1]);
        B *= 2;
    }
}
