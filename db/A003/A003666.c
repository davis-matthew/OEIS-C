/*
 * A003666  a(n) is smallest number which is uniquely of the form a(j) + a(k)
 *          with 1 <= j < k < n and a(1) = 1, a(2) = 4.
 * https://oeis.org/A003666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Ulam-type sieve: a(1) = 1, a(2) = 4; a(n) is the smallest s >
 *            a(n-1) such that s = a(j) + a(k) (j < k < n) in exactly one way;
 *            tracked with an incremental pair-sum count array
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static long long *vals = NULL;
static idx_t nvals = 0, vcap = 0;
static unsigned char *cnt = NULL;
static size_t csz = 0;

static void ensure_cnt(size_t need) {
    if (need <= csz) return;
    size_t nsz = csz ? csz : 1024;
    while (nsz < need) nsz *= 2;
    cnt = (unsigned char *)realloc(cnt, nsz);
    memset(cnt + csz, 0, nsz - csz);
    csz = nsz;
}

static void add_term(long long t) {
    ensure_cnt((size_t)(2 * t + 2));
    for (idx_t i = 0; i < nvals; i++) {
        size_t s = (size_t)(vals[i] + t);
        if (cnt[s] < 2) cnt[s]++;
    }
    if (nvals == vcap) {
        vcap = vcap ? vcap * 2 : 1024;
        vals = (long long *)realloc(vals, vcap * sizeof(long long));
    }
    vals[nvals++] = t;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (nvals == 0) {
        add_term(1);
        add_term(4);
    }
    while (nvals < n) {
        long long last = vals[nvals - 1];
        long long s = last + 1;
        while ((size_t)s < csz && cnt[s] != 1) s++;
        add_term(s);
    }
    return (seq_t)vals[n - 1];
}
