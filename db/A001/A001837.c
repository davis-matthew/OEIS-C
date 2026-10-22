/*
 * A001837  Numbers k such that phi(2k+1) < phi(2k).
 * https://oeis.org/A001837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th k >= 1 with phi(2k+1) < phi(2k); phi is
 *            computed by a totient sieve whose bound doubles until n terms
 *            are found
 * Range:     u8 n <= 1, u16 n <= 1050, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *a1837_terms = NULL;
static size_t a1837_nterms = 0;
static uint64_t a1837_lim = 0;

static void a1837_build(uint64_t L) {
    uint32_t *ph = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    uint64_t i, j, k;
    size_t cap = 1024, cnt = 0;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 0; i <= L; i++) ph[i] = (uint32_t)i;
    for (i = 2; i <= L; i++) {
        if (ph[i] == i) {
            for (j = i; j <= L; j += i) ph[j] -= ph[j] / (uint32_t)i;
        }
    }
    for (k = 1; 2 * k + 1 <= L; k++) {
        if (ph[2 * k + 1] < ph[2 * k]) {
            if (cnt == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, cap * sizeof(uint64_t));
            }
            t[cnt++] = k;
        }
    }
    free(ph);
    free(a1837_terms);
    a1837_terms = t;
    a1837_nterms = cnt;
    a1837_lim = L;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while ((idx_t)a1837_nterms < n) {
        uint64_t L = a1837_lim ? a1837_lim * 2 : ((uint64_t)1 << 16);
        a1837_build(L);
    }
    return (seq_t)a1837_terms[n - 1];
}
