/*
 * A004431  Numbers that are the sum of 2 distinct nonzero squares.
 * https://oeis.org/A004431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve: mark every x^2 + y^2 with 0 < x < y, then a(n) is the
 *            n-th marked number
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A004431_terms = NULL;
static uint64_t A004431_count = 0;
static uint64_t A004431_limit = 0;

static void A004431_sieve(uint64_t L) {
    unsigned char *mark = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t x, y, v, cnt = 0;
    for (x = 1; 2 * x * x < L; x++) {
        for (y = x + 1; (v = x * x + y * y) <= L; y++) {
            mark[v] = 1;
        }
    }
    for (v = 1; v <= L; v++)
        if (mark[v]) cnt++;
    free(A004431_terms);
    A004431_terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (v = 1; v <= L; v++)
        if (mark[v]) A004431_terms[cnt++] = v;
    free(mark);
    A004431_count = cnt;
    A004431_limit = L;
}

seq_t a(idx_t n) {
    if (A004431_limit == 0) A004431_sieve(1024);
    while (A004431_count < (uint64_t)n) A004431_sieve(A004431_limit * 2);
    return (seq_t)A004431_terms[n - 1];
}
