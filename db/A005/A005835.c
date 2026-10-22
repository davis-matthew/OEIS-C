/*
 * A005835  Pseudoperfect (or semiperfect) numbers k: some subset of the
 *          proper divisors of k sums to k.
 * https://oeis.org/A005835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   sieve. A k is a term if some subset of its proper divisors sums
 *            to k (checked by subset-sum DP on a bitset). Multiples of terms
 *            are terms, so only abundant/perfect k that are not yet marked
 *            are actually tested
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_pseudoperfect, subset_sum_hits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_pseudoperfect.h"
#include "../utils/subset_sum_hits.h"

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static uint64_t limit_done = 0;

static void build(uint64_t L) {
    uint64_t *sig = (uint64_t *)calloc((size_t)L + 1, sizeof(uint64_t));
    unsigned char *mark = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t d, m, k;
    idx_t cnt = 0;
    if (!sig || !mark) { free(sig); free(mark); return; }
    for (d = 1; d <= L / 2; d++)
        for (m = 2 * d; m <= L; m += d) sig[m] += d;
    for (k = 2; k <= L; k++) {
        if (mark[k]) { cnt++; continue; }
        if (sig[k] < k) continue;
        if (is_pseudoperfect(k)) {
            for (m = k; m <= L; m += k) mark[m] = 1;
            cnt++;
        }
    }
    free(terms);
    terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    nterms = 0;
    for (k = 2; k <= L; k++)
        if (mark[k]) terms[nterms++] = k;
    limit_done = L;
    free(sig);
    free(mark);
}

seq_t a(idx_t n) {
    while (nterms < n) {
        uint64_t L = limit_done ? limit_done * 2 : (uint64_t)n * 5 + 200;
        if (L < (uint64_t)n * 5 + 200) L = (uint64_t)n * 5 + 200;
        build(L);
    }
    return (seq_t)terms[n - 1];
}
