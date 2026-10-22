/*
 * A001284  Numbers of form m*k with m+1 <= k <= 2m-1.
 * https://oeis.org/A001284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve: mark every m*k with m+1 <= k <= 2m-1 up to a limit, then
 *            a(n) is the n-th marked number in increasing order
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static unsigned long long *a001284_terms = NULL;
static idx_t a001284_cnt = 0;

static void a001284_build(unsigned long long L) {
    unsigned char *mark = (unsigned char *)calloc((size_t)(L + 1), 1);
    unsigned long long m, k, x;
    idx_t c = 0;
    for (m = 2; m * (m + 1) <= L; m++) {
        for (k = m + 1; k <= 2 * m - 1 && m * k <= L; k++) {
            mark[m * k] = 1;
        }
    }
    for (x = 1; x <= L; x++) if (mark[x]) c++;
    free(a001284_terms);
    a001284_terms = (unsigned long long *)malloc((size_t)(c + 1) * sizeof(unsigned long long));
    a001284_cnt = 0;
    for (x = 1; x <= L; x++) if (mark[x]) a001284_terms[a001284_cnt++] = x;
    free(mark);
}

seq_t a(idx_t n) {
    if (n > a001284_cnt) {
        unsigned long long L = 4ULL * (unsigned long long)n + 100;
        for (;;) {
            a001284_build(L);
            if (a001284_cnt >= n) break;
            L *= 2;
        }
    }
    return (seq_t)a001284_terms[n - 1];
}
