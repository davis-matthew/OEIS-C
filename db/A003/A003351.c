/*
 * A003351  Numbers that are the sum of 6 positive 5th powers.
 * https://oeis.org/A003351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all nondecreasing 6-tuples of positive 5th
 *            powers with sum <= X, mark them in a table, and double X until
 *            the table holds at least n terms; a(n) is the n-th marked value
 * Range:     u8 n <= 8, u16 n <= 1314, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3351_p5, a3351_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/a3351_p5.h"
#include "../utils/a3351_rec.h"

static uint64_t *a3351_terms = NULL;
static idx_t a3351_count = 0;
static uint64_t a3351_limit = 0;

static void a3351_build(uint64_t X) {
    unsigned char *mark = (unsigned char *)calloc((size_t)X + 1, 1);
    uint64_t v;
    idx_t cnt = 0;
    if (!mark) return;
    a3351_rec(6, 1, 0, X, mark);
    for (v = 0; v <= X; v++)
        if (mark[v]) cnt++;
    free(a3351_terms);
    a3351_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    a3351_count = 0;
    if (!a3351_terms) {
        free(mark);
        return;
    }
    for (v = 0; v <= X; v++)
        if (mark[v]) a3351_terms[a3351_count++] = v;
    a3351_limit = X;
    free(mark);
}

seq_t a(idx_t n) {
    while (a3351_count < n) {
        uint64_t X = a3351_limit ? a3351_limit * 2 : ((uint64_t)1 << 16);
        a3351_build(X);
        if (!a3351_terms) return 0;
    }
    return (seq_t)a3351_terms[n - 1];
}
