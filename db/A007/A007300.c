/*
 * A007300  a(1)=2, a(2)=5; for n >= 3, a(n) is smallest number which is
 *          uniquely of the form a(j) + a(k) with 1 <= j < k < n.
 * https://oeis.org/A007300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2, a(2) = 5; a(n) = smallest number > a(n-1) with exactly
 *            one representation a(j)+a(k), 1 <= j < k < n (computed by an
 *            incremental Ulam-type sieve over pair-sum counts)
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static long long *terms = NULL;   /* terms[1..nterms] */
static idx_t nterms = 0;
static idx_t terms_cap = 0;
static unsigned char *cnt = NULL; /* cnt[s] = min(2, #representations of s) */
static long long cnt_cap = 0;

static void ensure_cnt(long long need) {
    if (need < cnt_cap) return;
    long long nc = cnt_cap ? cnt_cap : 1024;
    while (nc <= need) nc *= 2;
    unsigned char *p = (unsigned char *)realloc(cnt, (size_t)nc);
    memset(p + cnt_cap, 0, (size_t)(nc - cnt_cap));
    cnt = p;
    cnt_cap = nc;
}

static void ensure_terms(idx_t need) {
    if (need < terms_cap) return;
    idx_t nc = terms_cap ? terms_cap : 64;
    while (nc <= need) nc *= 2;
    terms = (long long *)realloc(terms, (size_t)nc * sizeof(long long));
    terms_cap = nc;
}

static void add_term(long long t) {
    ensure_cnt(2 * t + 2);
    for (idx_t i = 1; i <= nterms; i++) {
        long long s = terms[i] + t;
        if (cnt[s] < 2) cnt[s]++;
    }
    nterms++;
    ensure_terms(nterms + 1);
    terms[nterms] = t;
}

seq_t a(idx_t n) {
    if (nterms == 0) {
        ensure_terms(16);
        add_term(2);
        add_term(5);
    }
    while (nterms < n) {
        long long c = terms[nterms] + 1;
        ensure_cnt(2 * c + 2);
        while (cnt[c] != 1) {
            c++;
            ensure_cnt(2 * c + 2);
        }
        add_term(c);
    }
    return (seq_t)terms[n];
}
