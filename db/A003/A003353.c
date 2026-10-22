/*
 * A003353  Numbers that are the sum of 8 positive 5th powers.
 * https://oeis.org/A003353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve. Mark every sum of exactly k positive 5th powers up to a
 *            limit L for k = 1..8, list the sums with k = 8 in increasing
 *            order, and double L until the list has at least n terms
 * Range:     u8 n <= 9, u16 n <= 3744, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *A3353_terms = NULL;
static idx_t A3353_count = 0;

static void A3353_build(uint64_t L) {
    unsigned char *prev = (unsigned char *)malloc((size_t)(L + 1));
    unsigned char *cur = (unsigned char *)malloc((size_t)(L + 1));
    uint64_t p5[64];
    int nb = 0;
    uint64_t b;
    int k, i;
    uint64_t s;
    idx_t cnt = 0;
    for (b = 1; b < 64; b++) {
        uint64_t p = b * b * b * b * b;
        if (p > L) break;
        p5[nb++] = p;
    }
    memset(prev, 0, (size_t)(L + 1));
    prev[0] = 1;
    for (k = 1; k <= 8; k++) {
        unsigned char *t;
        memset(cur, 0, (size_t)(L + 1));
        for (s = 0; s <= L; s++) {
            if (!prev[s]) continue;
            for (i = 0; i < nb && s + p5[i] <= L; i++)
                cur[s + p5[i]] = 1;
        }
        t = prev; prev = cur; cur = t;
    }
    for (s = 0; s <= L; s++) if (prev[s]) cnt++;
    free(A3353_terms);
    A3353_terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    A3353_count = 0;
    for (s = 0; s <= L; s++) if (prev[s]) A3353_terms[A3353_count++] = s;
    free(prev);
    free(cur);
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (n < 1) n = 1;
    while (A3353_count < n) {
        L = (L == 0) ? ((uint64_t)1 << 16) : L * 2;
        A3353_build(L);
    }
    return (seq_t)A3353_terms[n - 1];
}
