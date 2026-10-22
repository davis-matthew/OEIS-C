/*
 * A004439  Numbers that are not the sum of 2 distinct nonzero squares.
 * https://oeis.org/A004439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- mark every i^2 + j^2 with 1 <= i < j up to a limit,
 *            list the unmarked positive integers in order, and double the
 *            limit until there are at least n of them; a(n) is the n-th one
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *a004439_list = NULL;
static uint64_t a004439_cnt = 0;
static uint64_t a004439_lim = 0;

static void a004439_build(uint64_t L) {
    unsigned char *mark = (unsigned char *)malloc((size_t)(L + 1));
    uint64_t i, j, k, c = 0;
    memset(mark, 0, (size_t)(L + 1));
    for (i = 1; 2 * i * i + 2 * i + 1 <= L; i++) {
        for (j = i + 1; i * i + j * j <= L; j++) {
            mark[i * i + j * j] = 1;
        }
    }
    for (k = 1; k <= L; k++) if (!mark[k]) c++;
    free(a004439_list);
    a004439_list = (uint64_t *)malloc((size_t)(c + 1) * sizeof(uint64_t));
    c = 0;
    for (k = 1; k <= L; k++) if (!mark[k]) a004439_list[c++] = k;
    free(mark);
    a004439_cnt = c;
    a004439_lim = L;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (a004439_cnt < n) {
        uint64_t L = a004439_lim ? a004439_lim : 1024;
        while (L < 2 * n) L *= 2;
        a004439_build(L);
        while (a004439_cnt < n) a004439_build(a004439_lim * 2);
    }
    return (seq_t)a004439_list[n - 1];
}
