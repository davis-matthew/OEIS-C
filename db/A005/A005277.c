/*
 * A005277  Nontotients: even numbers k such that phi(m) = k has no solution.
 * https://oeis.org/A005277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search -- a(n) is the n-th even k with no m such that
 *            phi(m) = k (m <= 8k suffices)
 * Range:     u8 n <= 41, u16 n <= 9900, u32 n <= 9900, u64 n <= 9900,
 *            u128 n <= 9900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *nt_list = NULL;
static uint64_t nt_count = 0;
static uint64_t nt_bound = 0;

static void nt_build(uint64_t bound) {
    uint64_t lim = 8 * bound;
    uint32_t *phi = (uint32_t *)malloc((lim + 1) * sizeof(uint32_t));
    uint8_t *seen = (uint8_t *)calloc(bound + 1, 1);
    uint64_t i, j, k;
    for (i = 0; i <= lim; i++) phi[i] = (uint32_t)i;
    for (i = 2; i <= lim; i++) {
        if (phi[i] == i) {
            for (j = i; j <= lim; j += i) phi[j] -= phi[j] / (uint32_t)i;
        }
    }
    for (i = 1; i <= lim; i++) {
        if (phi[i] <= bound) seen[phi[i]] = 1;
    }
    free(phi);
    free(nt_list);
    nt_list = (uint32_t *)malloc((bound / 2 + 1) * sizeof(uint32_t));
    nt_count = 0;
    for (k = 2; k <= bound; k += 2) {
        if (!seen[k]) nt_list[nt_count++] = (uint32_t)k;
    }
    free(seen);
    nt_bound = bound;
}

seq_t a(idx_t n) {
    uint64_t bound;
    if (n == 0) return (seq_t)0;
    bound = nt_bound ? nt_bound : 4096;
    while (nt_count < n) {
        if (nt_bound >= bound) bound *= 2;
        nt_build(bound);
    }
    return (seq_t)nt_list[n - 1];
}
