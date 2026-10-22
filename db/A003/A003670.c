/*
 * A003670  a(n) is smallest number which is uniquely a(j)+a(k).
 * https://oeis.org/A003670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Ulam-type sequence with a(1) = 4, a(2) = 7; computed by a sieve
 *            that counts how many ways each number is a sum of two distinct
 *            earlier terms, and takes the smallest number above the last term
 *            that has exactly one way
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static long long *terms = NULL;   /* terms[1..len] */
static idx_t len = 0, tcap = 0;
static unsigned char *cnt = NULL; /* cnt[v] = min(#representations, 2) */
static size_t ccap = 0;

static void ensure_cnt(size_t need) {
    if (need <= ccap) return;
    size_t nc = ccap ? ccap : 1024;
    while (nc < need) nc *= 2;
    cnt = (unsigned char *)realloc(cnt, nc);
    memset(cnt + ccap, 0, nc - ccap);
    ccap = nc;
}

static void add_term(long long t) {
    if (len + 1 >= tcap) {
        tcap = tcap ? tcap * 2 : 1024;
        terms = (long long *)realloc(terms, (size_t)tcap * sizeof(long long));
    }
    ensure_cnt((size_t)(2 * t + 2));
    for (idx_t i = 1; i <= len; i++) {
        size_t s = (size_t)(terms[i] + t);
        if (cnt[s] < 2) cnt[s]++;
    }
    len++;
    terms[len] = t;
}

static void ensure_computed(idx_t n) {
    if (len == 0) {
        add_term(4);
        add_term(7);
    }
    while (len < n) {
        long long v = terms[len] + 1;
        while (cnt[(size_t)v] != 1) v++;
        add_term(v);
    }
}

seq_t a(idx_t n) {
    ensure_computed(n);
    return (seq_t)terms[n];
}
