/*
 * A079698  Values of the odd part of A005277(n).
 * https://oeis.org/A079698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search -- a(n) is the (n+1)-th odd m such that 2m is a *
 *            nontotient (A005277). If 2^k*m = phi(x), then 2^(k+1)*m =
 *            phi(2x) or * phi(4x), so the odd parts of the nontotients are
 *            exactly the odd m with * 2m a nontotient, i.e. the nontotients
 *            == 2 (mod 4) divided by 2
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A079698_list = NULL;
static uint64_t A079698_count = 0;
static uint64_t A079698_bound = 0;

/* Sieve phi up to 8*bound, mark totient values <= bound, collect odd m
   with 2m <= bound and 2m not a totient. */
static void A079698_build(uint64_t bound) {
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
    free(A079698_list);
    A079698_list = (uint32_t *)malloc((bound / 4 + 2) * sizeof(uint32_t));
    A079698_count = 0;
    for (k = 2; k <= bound; k += 4) {
        if (!seen[k]) A079698_list[A079698_count++] = (uint32_t)(k / 2);
    }
    free(seen);
    A079698_bound = bound;
}

seq_t a(idx_t n) {
    uint64_t need = (uint64_t)n + 1;
    uint64_t bound = A079698_bound ? A079698_bound : 4096;
    while (A079698_count < need) {
        if (A079698_bound >= bound) bound *= 2;
        A079698_build(bound);
    }
    return (seq_t)A079698_list[n];
}
