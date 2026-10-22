/*
 * A007086  Next term is uniquely the sum of 3 earlier terms.
 * https://oeis.org/A007086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(1)=1, a(2)=2, a(3)=3; for n>3, a(n) is the least
 *            number greater than a(n-1) that is the sum of three distinct
 *            earlier terms in exactly one way. Representation counts (capped
 *            at 2) are kept in an array of pair-sum counts and an array of
 *            triple-sum counts, updated each time a term is added
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_bytes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ensure_bytes.h"

static int64_t *T = NULL;
static size_t nt = 0, capT = 0;
static unsigned char *P = NULL;   /* P[s] = #pairs of distinct terms summing to s, capped at 2 */
static size_t capP = 0;
static unsigned char *C = NULL;   /* C[s] = #triples of distinct terms summing to s, capped at 2 */
static size_t capC = 0;

static void add_term(int64_t t) {
    size_t i;
    /* new triples: t + (pair of earlier terms) */
    if (nt >= 2) {
        size_t maxs = (size_t)(T[nt - 1] + T[nt - 2]);
        C = ensure_bytes(C, &capC, (size_t)t + maxs + 1);
        for (size_t s = 3; s <= maxs; s++) {
            unsigned char p = P[s];
            if (p) {
                unsigned v = (unsigned)C[t + s] + p;
                C[t + s] = (unsigned char)(v > 2 ? 2 : v);
            }
        }
    }
    /* new pairs: t + earlier term */
    P = ensure_bytes(P, &capP, (size_t)(2 * t) + 1);
    for (i = 0; i < nt; i++) {
        size_t s = (size_t)(t + T[i]);
        if (P[s] < 2) P[s]++;
    }
    if (nt == capT) {
        capT = capT ? capT * 2 : 1024;
        T = (int64_t *)realloc(T, capT * sizeof(int64_t));
    }
    T[nt++] = t;
}

static void compute_up_to(idx_t n) {
    if (nt == 0) {
        add_term(1);
        add_term(2);
        add_term(3);
    }
    while ((idx_t)nt < n) {
        int64_t t = T[nt - 1];
        C = ensure_bytes(C, &capC, (size_t)(3 * t) + 8);
        int64_t c = t + 1;
        while (1) {
            if ((size_t)c >= capC) C = ensure_bytes(C, &capC, (size_t)c + 1);
            if (C[c] == 1) break;
            c++;
        }
        add_term(c);
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    compute_up_to(n);
    return (seq_t)T[n - 1];
}
