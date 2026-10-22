/*
 * A004210  "Magic" integers: a(n+1) is the smallest integer m such that there
 *          is no overlap between the sets {m, m-a(i), m+a(i): 1 <= i <= n}
 *          and {a(i), a(i)-a(j), a(i)+a(j): 1 <= j < i <= n}.
 * https://oeis.org/A004210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   search. a(1) = 1; a(n+1) = smallest m > a(n) such that m, m-a(i)
 *            and m+a(i) are all outside F = {a(i), a(i)-a(j), a(i)+a(j): j <
 *            i <= n}. F is kept as a bitset
 * Range:     u8 n <= 11, u16 n <= 112, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static long long *terms = NULL;
static idx_t tlen = 0, tcap = 0;
static uint64_t *F = NULL;
static size_t fwords = 0;

static void fset(long long x) {
    size_t w = (size_t)(x >> 6);
    if (w >= fwords) {
        size_t nw = fwords ? fwords : 1024;
        while (nw <= w) nw *= 2;
        F = (uint64_t *)realloc(F, nw * sizeof(uint64_t));
        memset(F + fwords, 0, (nw - fwords) * sizeof(uint64_t));
        fwords = nw;
    }
    F[w] |= (uint64_t)1 << (x & 63);
}

static int fget(long long x) {
    size_t w = (size_t)(x >> 6);
    if (w >= fwords) return 0;
    return (int)((F[w] >> (x & 63)) & 1);
}

static void add_term(long long v) {
    idx_t j;
    if (tlen >= tcap) {
        tcap = tcap ? tcap * 2 : 1024;
        terms = (long long *)realloc(terms, tcap * sizeof(long long));
    }
    fset(v);
    for (j = 0; j < tlen; j++) {
        fset(v - terms[j]);
        fset(v + terms[j]);
    }
    terms[tlen++] = v;
}

static void compute_next(void) {
    long long m = terms[tlen - 1] + 1;
    for (;; m++) {
        bool ok = true;
        idx_t i;
        if (fget(m)) continue;
        for (i = tlen; i-- > 0;) {
            long long t = terms[i];
            if (fget(m + t) || (m > t && fget(m - t))) { ok = false; break; }
        }
        if (ok) break;
    }
    add_term(m);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (tlen == 0) add_term(1);
    while (tlen < n) compute_next();
    return (seq_t)terms[n - 1];
}
