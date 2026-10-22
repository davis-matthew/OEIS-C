/*
 * A002859  a(1) = 1, a(2) = 3; for n >= 3, a(n) is smallest number that is
 *          uniquely of the form a(j) + a(k) with 1 <= j < k < n.
 * https://oeis.org/A002859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1) = 1, a(2) = 3; for n >= 3 the code searches for a(n). It
 *            keeps a sieve cnt[s] of how many pairs a(j)+a(k), j<k, sum to s
 *            (counts capped at 2). a(n) is the smallest s > a(n-1) with
 *            cnt[s] == 1
 * Range:     u8 n <= 58, u16 n <= 8377, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static long long *a2859_t = NULL;
static idx_t a2859_len = 0, a2859_tcap = 0;
static unsigned char *a2859_cnt = NULL;
static size_t a2859_ccap = 0;

static void a2859_ensure_cnt(size_t need) {
    if (need <= a2859_ccap) return;
    size_t nc = a2859_ccap ? a2859_ccap : 1024;
    while (nc < need) nc *= 2;
    a2859_cnt = (unsigned char *)realloc(a2859_cnt, nc);
    memset(a2859_cnt + a2859_ccap, 0, nc - a2859_ccap);
    a2859_ccap = nc;
}

static void a2859_push(long long x) {
    if (a2859_len >= a2859_tcap) {
        a2859_tcap = a2859_tcap ? a2859_tcap * 2 : 256;
        a2859_t = (long long *)realloc(a2859_t, a2859_tcap * sizeof(long long));
    }
    a2859_ensure_cnt((size_t)(2 * x + 2));
    for (idx_t i = 0; i < a2859_len; i++) {
        size_t s = (size_t)(a2859_t[i] + x);
        if (a2859_cnt[s] < 2) a2859_cnt[s]++;
    }
    a2859_t[a2859_len++] = x;
}

seq_t a(idx_t n) {
    if (a2859_len == 0) {
        a2859_push(1);
        a2859_push(3);
    }
    while (a2859_len < n) {
        long long last = a2859_t[a2859_len - 1];
        long long s = last + 1;
        while (a2859_cnt[s] != 1) s++;
        a2859_push(s);
    }
    return (seq_t)a2859_t[n - 1];
}
