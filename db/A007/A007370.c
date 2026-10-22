/*
 * A007370  Numbers k such that sigma(x) = k has a unique solution.
 * https://oeis.org/A007370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve. Compute sigma(x) for all x <= L and count how many x give
 *            each value k <= L (any solution x of sigma(x) = k has x <= k).
 *            The terms are the k with exactly one solution. Double L until
 *            there are enough terms
 * Range:     u8 n <= 52, u16 n <= 8206, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a7370_terms = NULL;
static idx_t a7370_count = 0;
static uint64_t a7370_limit = 0;

static void a7370_build(uint64_t L) {
    uint64_t *sig = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    unsigned char *cnt = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t d, m, k;
    idx_t c = 0;
    for (d = 1; d <= L; d++)
        for (m = d; m <= L; m += d)
            sig[m] += d;
    for (m = 1; m <= L; m++) {
        uint64_t s = sig[m];
        if (s <= L && cnt[s] < 2) cnt[s]++;
    }
    for (k = 1; k <= L; k++)
        if (cnt[k] == 1) c++;
    free(a7370_terms);
    a7370_terms = (uint64_t *)malloc((size_t)(c + 1) * sizeof(uint64_t));
    c = 0;
    for (k = 1; k <= L; k++)
        if (cnt[k] == 1) a7370_terms[c++] = k;
    a7370_count = c;
    a7370_limit = L;
    free(sig);
    free(cnt);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (a7370_count < n) {
        uint64_t L = a7370_limit ? a7370_limit : 1024;
        while (L < (uint64_t)n * 4) L *= 2;
        a7370_build(L);
        while (a7370_count < n) a7370_build(a7370_limit * 2);
    }
    return (seq_t)a7370_terms[n - 1];
}
