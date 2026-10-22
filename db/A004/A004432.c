/*
 * A004432  Numbers that are the sum of 3 distinct nonzero squares.
 * https://oeis.org/A004432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve: mark every x^2 + y^2 + z^2 <= L with 0 < x < y < z, list the marked numbers in increasing order, and double L until there are at least n terms */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static idx_t terms_len = 0;
static uint64_t sieve_limit = 0;

static void build(uint64_t L) {
    unsigned char *mark = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t x, y, z, cnt = 0, v;
    for (x = 1; x * x + (x + 1) * (x + 1) + (x + 2) * (x + 2) <= L; x++) {
        for (y = x + 1; x * x + y * y + (y + 1) * (y + 1) <= L; y++) {
            uint64_t base = x * x + y * y;
            for (z = y + 1; base + z * z <= L; z++) {
                mark[base + z * z] = 1;
            }
        }
    }
    for (v = 0; v <= L; v++) if (mark[v]) cnt++;
    free(terms);
    terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    terms_len = 0;
    for (v = 0; v <= L; v++) if (mark[v]) terms[terms_len++] = v;
    free(mark);
    sieve_limit = L;
}

seq_t a(idx_t n) {
    if (terms == NULL || terms_len < n) {
        uint64_t L = sieve_limit ? sieve_limit : 64;
        if (L < 2 * (uint64_t)n + 64) L = 2 * (uint64_t)n + 64;
        build(L);
        while (terms_len < n) {
            L *= 2;
            build(L);
        }
    }
    return (seq_t)terms[n - 1];
}
