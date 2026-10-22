/*
 * A003667  a(n) is smallest number which is uniquely of the form a(j) + a(k)
 *          with 1 <= j < k < n and a(1) = 1, a(2) = 5.
 * https://oeis.org/A003667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form; computed by a sieve. Keep a count of the
 *            pairwise sums a(j)+a(k), j<k, capped at 2. a(n) is the smallest
 *            number above a(n-1) whose count is exactly 1. a(1) = 1, a(2) = 5
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned long long *A3667_t = NULL;
static idx_t A3667_len = 0, A3667_cap = 0;
static unsigned char *A3667_cnt = NULL;
static size_t A3667_ccap = 0;

static void A3667_ensure_cnt(size_t need) {
    if (need <= A3667_ccap) return;
    size_t nc = A3667_ccap ? A3667_ccap : 1024;
    while (nc < need) nc *= 2;
    A3667_cnt = (unsigned char *)realloc(A3667_cnt, nc);
    memset(A3667_cnt + A3667_ccap, 0, nc - A3667_ccap);
    A3667_ccap = nc;
}

static void A3667_push(unsigned long long v) {
    if (A3667_len >= A3667_cap) {
        A3667_cap = A3667_cap ? A3667_cap * 2 : 1024;
        A3667_t = (unsigned long long *)realloc(A3667_t, A3667_cap * sizeof(unsigned long long));
    }
    A3667_ensure_cnt((size_t)(2 * v + 2));
    for (idx_t i = 0; i < A3667_len; i++) {
        unsigned long long s = A3667_t[i] + v;
        if (A3667_cnt[s] < 2) A3667_cnt[s]++;
    }
    A3667_t[A3667_len++] = v;
}

seq_t a(idx_t n) {
    if (A3667_len == 0) {
        A3667_push(1);
        A3667_push(5);
    }
    while (A3667_len < n) {
        unsigned long long last = A3667_t[A3667_len - 1];
        unsigned long long v = last + 1;
        while (v < (unsigned long long)A3667_ccap && A3667_cnt[v] != 1) v++;
        A3667_push(v);
    }
    return (seq_t)(uint64_t)A3667_t[n - 1];
}
