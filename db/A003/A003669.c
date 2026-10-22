/*
 * A003669  a(n) is smallest number which is uniquely a(j) + a(k), j < k.
 * https://oeis.org/A003669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by an Ulam-type sieve: a(1)=3, a(2)=4; a(n) is the
 *            smallest x > a(n-1) with exactly one representation x =
 *            a(j)+a(k), j<k
 * Range:     u8 n <= 58, u16 n <= 7918, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static long long *A003669_terms = NULL;
static idx_t A003669_len = 0, A003669_tcap = 0;
static unsigned char *A003669_cnt = NULL;
static long long A003669_ccap = 0;

static void A003669_ensure_cnt(long long need) {
    if (need < A003669_ccap) return;
    long long nc = A003669_ccap ? A003669_ccap : 1024;
    while (nc <= need) nc *= 2;
    A003669_cnt = (unsigned char *)realloc(A003669_cnt, (size_t)nc);
    memset(A003669_cnt + A003669_ccap, 0, (size_t)(nc - A003669_ccap));
    A003669_ccap = nc;
}

static void A003669_push(long long t) {
    if (A003669_len >= A003669_tcap) {
        A003669_tcap = A003669_tcap ? A003669_tcap * 2 : 1024;
        A003669_terms = (long long *)realloc(A003669_terms, sizeof(long long) * (size_t)A003669_tcap);
    }
    A003669_ensure_cnt(2 * t + 2);
    for (idx_t i = 0; i < A003669_len; i++) {
        long long s = t + A003669_terms[i];
        if (A003669_cnt[s] < 2) A003669_cnt[s]++;
    }
    A003669_terms[A003669_len++] = t;
}

seq_t a(idx_t n) {
    if (A003669_len == 0) {
        A003669_push(3);
        A003669_push(4);
    }
    while (A003669_len < n) {
        long long x = A003669_terms[A003669_len - 1] + 1;
        while (A003669_cnt[x] != 1) x++;
        A003669_push(x);
    }
    return (seq_t)(unsigned long long)A003669_terms[n - 1];
}
