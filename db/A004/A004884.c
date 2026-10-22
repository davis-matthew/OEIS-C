/*
 * A004884  Numbers that are the sum of at most 11 nonzero 8th powers.
 * https://oeis.org/A004884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- enumerate all sums of at most 11 nonzero 8th powers up
 *            to a bound L (multisets of bases), sort and dedupe, and increase
 *            L until at least n distinct values exist; a(n) is the n-th
 *            smallest (a(1) = 0, the empty sum)
 * Range:     u8 n <= 12, u16 n <= 360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A_cmp, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/u128_to_seq.h"
#include "../utils/A_cmp.h"

static uint64_t *A_vals = NULL;
static size_t A_nvals = 0;

static uint64_t A_pw[256];
static int A_np = 0;
static uint64_t *A_buf = NULL;
static size_t A_blen = 0, A_bcap = 0;

static void A_push(uint64_t v) {
    if (A_blen == A_bcap) {
        size_t nc = A_bcap ? A_bcap * 2 : 4096;
        A_buf = (uint64_t *)realloc(A_buf, nc * sizeof(uint64_t));
        A_bcap = nc;
    }
    A_buf[A_blen++] = v;
}

static void A_gen(int start, int rem, uint64_t s, uint64_t L) {
    int i;
    A_push(s);
    if (rem == 0) return;
    for (i = start; i < A_np; i++) {
        if (A_pw[i] > L - s) break;
        A_gen(i, rem - 1, s + A_pw[i], L);
    }
}

static void A_build(uint64_t L) {
    uint64_t b;
    size_t i, k;
    A_np = 0;
    for (b = 1; A_np < 256; b++) {
        uint64_t p = b * b;
        p = p * p;
        p = p * p;
        if (p > L) break;
        A_pw[A_np++] = p;
    }
    A_blen = 0;
    A_gen(0, 11, 0, L);
    qsort(A_buf, A_blen, sizeof(uint64_t), A_cmp);
    k = 0;
    for (i = 0; i < A_blen; i++)
        if (k == 0 || A_buf[i] != A_buf[k - 1]) A_buf[k++] = A_buf[i];
    free(A_vals);
    A_vals = (uint64_t *)malloc((k ? k : 1) * sizeof(uint64_t));
    memcpy(A_vals, A_buf, k * sizeof(uint64_t));
    A_nvals = k;
    free(A_buf);
    A_buf = NULL;
    A_blen = A_bcap = 0;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (A_vals == NULL || (idx_t)A_nvals < n) {
        uint64_t L = (uint64_t)1 << 16;
        for (;;) {
            A_build(L);
            if ((idx_t)A_nvals >= n) break;
            L *= 4;
        }
    }
    return u128_to_seq((unsigned __int128)A_vals[n - 1]);
}
