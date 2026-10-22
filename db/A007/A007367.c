/*
 * A007367  Numbers k such that phi(x) = k has exactly 3 solutions.
 * https://oeis.org/A007367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve phi(x) for x <= 7*L and count preimages of each k <= L
 *            (x/phi(x) < 7 for x < 6e9); a(n) = n-th k with exactly 3
 *            preimages, L doubled as needed
 * Range:     u8 n <= 10, u16 n <= 1884, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *terms = 0;
static size_t nterms = 0;

static void compute(uint64_t L) {
    uint64_t X = 7 * L, x, p, m;
    uint32_t *ph = (uint32_t *)malloc((size_t)(X + 1) * sizeof(uint32_t));
    unsigned char *cnt = (unsigned char *)calloc((size_t)(L + 1), 1);
    size_t cap = 1024, k;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t nt = 0;
    for (x = 0; x <= X; x++) ph[x] = (uint32_t)x;
    for (p = 2; p <= X; p++) {
        if (ph[p] == p) {
            for (m = p; m <= X; m += p) ph[m] -= ph[m] / (uint32_t)p;
        }
    }
    for (x = 1; x <= X; x++) {
        uint64_t v = ph[x];
        if (v <= L && cnt[v] < 255) cnt[v]++;
    }
    for (k = 1; k <= L; k++) {
        if (cnt[k] == 3) {
            if (nt == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, cap * sizeof(uint64_t));
            }
            t[nt++] = (uint64_t)k;
        }
    }
    free(ph);
    free(cnt);
    if (terms) free(terms);
    terms = t;
    nterms = nt;
}

seq_t a(idx_t n) {
    uint64_t L = 4096;
    while (nterms < n) {
        compute(L);
        L *= 2;
    }
    return (seq_t)terms[n - 1];
}
