/*
 * A003329  Numbers that are the sum of 6 positive cubes.
 * https://oeis.org/A003329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve. DP over k = 0..6 marks every m <= L that is a sum of
 *            exactly k positive cubes; the terms are the m marked at k = 6,
 *            listed in increasing order. L doubles until there are at least n
 *            terms
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static long long *a3329_terms = NULL;
static long long a3329_count = 0;

static void a3329_build(long long L) {
    unsigned char *cur = (unsigned char *)malloc((size_t)(L + 1));
    unsigned char *nxt = (unsigned char *)malloc((size_t)(L + 1));
    long long cubes[2000];
    int nc = 0;
    long long m, k, i, cnt;
    for (i = 1; i * i * i <= L; i++) cubes[nc++] = i * i * i;
    memset(cur, 0, (size_t)(L + 1));
    cur[0] = 1;
    for (k = 0; k < 6; k++) {
        memset(nxt, 0, (size_t)(L + 1));
        for (m = 0; m <= L; m++) {
            if (!cur[m]) continue;
            for (i = 0; i < nc && m + cubes[i] <= L; i++)
                nxt[m + cubes[i]] = 1;
        }
        { unsigned char *t = cur; cur = nxt; nxt = t; }
    }
    cnt = 0;
    for (m = 1; m <= L; m++) if (cur[m]) cnt++;
    free(a3329_terms);
    a3329_terms = (long long *)malloc((size_t)(cnt + 1) * sizeof(long long));
    cnt = 0;
    for (m = 1; m <= L; m++) if (cur[m]) a3329_terms[cnt++] = m;
    a3329_count = cnt;
    free(cur);
    free(nxt);
}

seq_t a(idx_t n) {
    long long L = 1024;
    if (n < 1) return 0;
    while ((idx_t)a3329_count < n) {
        while (L < 2 * (long long)n + 1024) L *= 2;
        a3329_build(L);
        L *= 2;
    }
    return (seq_t)a3329_terms[n - 1];
}
