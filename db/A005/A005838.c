/*
 * A005838  Lexicographically earliest increasing sequence of positive numbers
 *          that contains no arithmetic progression of length 6.
 * https://oeis.org/A005838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   greedy search. Values are added in increasing order, so any new
 *            6-term AP must end at the new term. When x is added, each 5-term
 *            AP x-4d,...,x makes x+d forbidden; a(n) is the smallest
 *            non-forbidden value above a(n-1)
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static long long *terms = NULL;
static idx_t tlen = 0, tcap = 0;
static unsigned char *pres = NULL, *forb = NULL;
static long long vcap = 0;

static void ensure_val(long long need) {
    if (need < vcap) return;
    long long nc = vcap ? vcap : 1024;
    while (nc <= need) nc *= 2;
    pres = (unsigned char *)realloc(pres, (size_t)nc);
    forb = (unsigned char *)realloc(forb, (size_t)nc);
    memset(pres + vcap, 0, (size_t)(nc - vcap));
    memset(forb + vcap, 0, (size_t)(nc - vcap));
    vcap = nc;
}

static void add_term(void) {
    long long c = tlen ? terms[tlen - 1] + 1 : 1;
    for (;;) {
        ensure_val(2 * c + 2);
        if (!forb[c]) break;
        c++;
    }
    if (tlen == tcap) {
        tcap = tcap ? tcap * 2 : 1024;
        terms = (long long *)realloc(terms, (size_t)tcap * sizeof(long long));
    }
    pres[c] = 1;
    for (idx_t i = tlen; i > 0; i--) {
        long long d = c - terms[i - 1];
        if (c - 4 * d < 1) break;
        if (pres[c - 2 * d] && pres[c - 3 * d] && pres[c - 4 * d])
            forb[c + d] = 1;
    }
    terms[tlen++] = c;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (tlen < n) add_term();
    return (seq_t)terms[n - 1];
}
