/*
 * A007530  Prime quadruples: numbers k such that k, k+2, k+6, k+8 are all
 *          prime.
 * https://oeis.org/A007530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   segmented sieve of Eratosthenes over odd numbers; a(n) is the
 *            n-th k with k, k+2, k+6, k+8 all prime. Found terms are memoized
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   q_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q_isqrt.h"

static uint64_t *q_terms = NULL;
static idx_t q_count = 0, q_cap = 0;
static uint64_t q_lo = 3;            /* next odd number to scan as k */

static uint32_t *q_bp = NULL;        /* odd base primes */
static size_t q_nbp = 0;
static uint64_t q_bound = 0;         /* base primes cover all p <= q_bound */

static void q_base(uint64_t need) {
    uint64_t b, i, j;
    unsigned char *c;
    size_t cnt = 0;
    if (q_bound >= need) return;
    b = q_bound ? q_bound : 1024;
    while (b < need) b *= 2;
    c = (unsigned char *)calloc((size_t)b + 1, 1);
    for (i = 2; i * i <= b; i++)
        if (!c[i])
            for (j = i * i; j <= b; j += i) c[j] = 1;
    free(q_bp);
    for (i = 3; i <= b; i += 2) if (!c[i]) cnt++;
    q_bp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    q_nbp = 0;
    for (i = 3; i <= b; i += 2) if (!c[i]) q_bp[q_nbp++] = (uint32_t)i;
    free(c);
    q_bound = b;
}

static void q_segment(void) {
    const uint64_t L0 = (uint64_t)1 << 18;  /* candidate k count (odd) */
    const uint64_t L = L0 + 4;              /* extra to cover k+8 */
    uint64_t lo = q_lo, hi = lo + 2 * (L - 1), i;
    unsigned char *comp;
    size_t t;
    q_base(q_isqrt(hi) + 1);
    comp = (unsigned char *)calloc((size_t)L, 1);
    for (t = 0; t < q_nbp; t++) {
        uint64_t p = q_bp[t], s;
        if (p * p > hi) break;
        s = ((lo + p - 1) / p) * p;
        if (s < p * p) s = p * p;
        if ((s & 1) == 0) s += p;
        for (i = (s - lo) / 2; i < L; i += p) comp[i] = 1;
    }
    for (i = 0; i < L0; i++) {
        if (!comp[i] && !comp[i + 1] && !comp[i + 3] && !comp[i + 4]) {
            if (q_count == q_cap) {
                q_cap = q_cap ? q_cap * 2 : 1024;
                q_terms = (uint64_t *)realloc(q_terms, (size_t)q_cap * sizeof(uint64_t));
            }
            q_terms[q_count++] = lo + 2 * i;
        }
    }
    free(comp);
    q_lo = lo + 2 * L0;
}

seq_t a(idx_t n) {
    while (q_count < n) q_segment();
    return (seq_t)q_terms[n - 1];
}
