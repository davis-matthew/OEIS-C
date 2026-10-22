/*
 * A006036  Primitive pseudoperfect numbers.
 * https://oeis.org/A006036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   sieve search. a(n) is the n-th number m that is pseudoperfect
 *            (some subset of its proper divisors sums to m) and has no
 *            pseudoperfect proper divisor. Equivalently, the proper divisors
 *            have a subset summing to sigma(m) - 2m, tested by a bitset
 *            subset-sum. Each primitive term marks all of its multi...
 * Range:     u8 n <= 5, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *pp_terms = NULL;
static idx_t pp_count = 0;
static uint64_t pp_limit = 0;

/* m has sum of proper divisors s = sigma(m) - m >= m; e = s - m.
   m is pseudoperfect iff some subset of its proper divisors sums to e. */
static bool is_pseudoperfect_u(uint64_t m, uint64_t e) {
    uint64_t *divs, nd = 0, cap = 64, W, i, k;
    uint64_t *bits;
    bool res = false;
    if (e == 0) return true;
    divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 1; i * i <= m; i++) {
        if (m % i == 0) {
            uint64_t c = m / i;
            if (nd + 2 > cap) {
                cap *= 2;
                divs = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
            }
            if (i < m && i <= e) divs[nd++] = i;
            if (c != i && c < m && c <= e) divs[nd++] = c;
        }
    }
    W = e / 64 + 1;
    bits = (uint64_t *)calloc((size_t)W, sizeof(uint64_t));
    bits[0] = 1;
    for (k = 0; k < nd && !res; k++) {
        uint64_t d = divs[k];
        uint64_t ws = d / 64, bs = d % 64;
        uint64_t idx;
        for (idx = W; idx-- > ws;) {
            uint64_t src = idx - ws;
            uint64_t v = bits[src] << bs;
            if (bs && src >= 1) v |= bits[src - 1] >> (64 - bs);
            bits[idx] |= v;
        }
        if ((bits[e / 64] >> (e % 64)) & 1) res = true;
    }
    free(bits);
    free(divs);
    return res;
}

static void pp_compute(uint64_t L) {
    uint64_t *sigma = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    unsigned char *mark = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t i, j, cap = 1024;
    free(pp_terms);
    pp_terms = (uint64_t *)malloc(cap * sizeof(uint64_t));
    pp_count = 0;
    for (i = 1; i <= L; i++)
        for (j = i; j <= L; j += i) sigma[j] += i;
    for (i = 2; i <= L; i++) {
        if (mark[i]) continue;
        if (sigma[i] < 2 * i) continue;
        if (is_pseudoperfect_u(i, sigma[i] - 2 * i)) {
            if (pp_count >= cap) {
                cap *= 2;
                pp_terms = (uint64_t *)realloc(pp_terms, cap * sizeof(uint64_t));
            }
            pp_terms[pp_count++] = i;
            for (j = 2 * i; j <= L; j += i) mark[j] = 1;
        }
    }
    free(sigma);
    free(mark);
    pp_limit = L;
}

seq_t a(idx_t n) {
    if (pp_limit == 0) pp_compute(4096);
    while (pp_count < n) pp_compute(pp_limit * 4);
    return (seq_t)pp_terms[n - 1];
}
