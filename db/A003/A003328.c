/*
 * A003328  Numbers that are the sum of 5 positive cubes.
 * https://oeis.org/A003328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve/DP: mark sums of exactly k positive cubes for k = 1..5 up
 *            to a limit, list marked numbers in increasing order (limit
 *            doubled as needed)
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *a3328_terms = NULL;
static size_t a3328_cnt = 0;
static size_t a3328_lim = 0;

static void a3328_build(size_t L) {
    unsigned char *cur = (unsigned char *)calloc(L + 1, 1);
    unsigned char *nxt = (unsigned char *)calloc(L + 1, 1);
    size_t s, c, i;
    int k;
    cur[0] = 1;
    for (k = 1; k <= 5; k++) {
        memset(nxt, 0, L + 1);
        for (s = 0; s <= L; s++) {
            if (!cur[s]) continue;
            for (c = 1; s + c * c * c <= L; c++) nxt[s + c * c * c] = 1;
        }
        { unsigned char *t = cur; cur = nxt; nxt = t; }
    }
    free(a3328_terms);
    a3328_cnt = 0;
    for (i = 0; i <= L; i++) if (cur[i]) a3328_cnt++;
    a3328_terms = (uint64_t *)malloc((a3328_cnt + 1) * sizeof(uint64_t));
    a3328_cnt = 0;
    for (i = 0; i <= L; i++) if (cur[i]) a3328_terms[a3328_cnt++] = (uint64_t)i;
    free(cur);
    free(nxt);
    a3328_lim = L;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (a3328_terms == NULL || (idx_t)a3328_cnt < n) {
        size_t L = a3328_lim ? a3328_lim * 2 : 4096;
        a3328_build(L);
    }
    return (seq_t)a3328_terms[n - 1];
}
