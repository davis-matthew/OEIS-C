/*
 * A003045  a(n) (n>6) is least integer > a(n-1) with precisely three
 *          representations a(n) = a(i) + a(j), 1 <= i < j < n, a(n) = n for
 *          n=1..6.
 * https://oeis.org/A003045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n for n=1..6; afterwards a(n) is the least k >
 *            a(n-1) whose count of pairs a(i)+a(j)=k (i<j<n) is exactly 3.
 *            The pair counts are kept in a sieve-style table that is updated
 *            each time a term is added
 * Range:     u8 n <= 52, u16 n <= 4261, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *A3045_t = 0;
static size_t A3045_len = 0, A3045_cap = 0;
static unsigned char *A3045_cnt = 0;
static size_t A3045_ccap = 0;

static void A3045_add(uint64_t v) {
    size_t need = (size_t)(2 * v + 2);
    size_t i;
    if (need > A3045_ccap) {
        size_t nc = A3045_ccap ? A3045_ccap : 64;
        while (nc < need) nc *= 2;
        A3045_cnt = (unsigned char *)realloc(A3045_cnt, nc);
        memset(A3045_cnt + A3045_ccap, 0, nc - A3045_ccap);
        A3045_ccap = nc;
    }
    if (A3045_len == A3045_cap) {
        size_t nc = A3045_cap ? 2 * A3045_cap : 64;
        A3045_t = (uint64_t *)realloc(A3045_t, nc * sizeof(uint64_t));
        A3045_cap = nc;
    }
    for (i = 0; i < A3045_len; i++) {
        size_t s = (size_t)(A3045_t[i] + v);
        if (A3045_cnt[s] < 4) A3045_cnt[s]++;
    }
    A3045_t[A3045_len++] = v;
}

seq_t a(idx_t n) {
    if (n <= 6) return (seq_t)n;
    if (A3045_len == 0) {
        uint64_t k;
        for (k = 1; k <= 6; k++) A3045_add(k);
    }
    while ((idx_t)A3045_len < n) {
        uint64_t last = A3045_t[A3045_len - 1];
        uint64_t c = last + 1;
        uint64_t lim = 2 * last + 1;
        while (c < lim && A3045_cnt[c] != 3) c++;
        if (c >= lim) return (seq_t)0; /* sequence would terminate */
        A3045_add(c);
    }
    return (seq_t)A3045_t[n - 1];
}
