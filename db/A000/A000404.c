/*
 * A000404  Numbers that are the sum of 2 nonzero squares.
 * https://oeis.org/A000404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve: mark every i^2 + j^2 with 1 <= i <= j up to a limit, list the marked numbers in increasing order, and double the limit until there are at least n terms. */
#include <stdint.h>
#include <stdlib.h>

static uint64_t *a404_terms = 0;
static idx_t a404_count = 0;
static uint64_t a404_limit = 0;

static void a404_build(uint64_t L) {
    unsigned char *mark = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t i, j, k;
    idx_t cnt = 0;
    for (i = 1; 2 * i * i <= L; i++)
        for (j = i; i * i + j * j <= L; j++)
            mark[i * i + j * j] = 1;
    for (k = 2; k <= L; k++)
        if (mark[k]) cnt++;
    free(a404_terms);
    a404_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    cnt = 0;
    for (k = 2; k <= L; k++)
        if (mark[k]) a404_terms[cnt++] = k;
    free(mark);
    a404_count = cnt;
    a404_limit = L;
}

seq_t a(idx_t n) {
    while (a404_count < n) {
        uint64_t L = a404_limit ? a404_limit * 2 : 4096;
        a404_build(L);
    }
    return (seq_t)a404_terms[n - 1];
}
