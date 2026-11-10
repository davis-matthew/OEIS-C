/*
 * A098699  Anti-derivative of n: or the first occurrence of n in A003415, or
 *          zero if impossible.
 * https://oeis.org/A098699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least x with A003415(x) = n (arithmetic derivative), else
 *            0; a(0)=1, a(1)=2. Computed by sieving A003415 up to x <= n^2/4
 * Range:     u8 n <= 53, u16 n <= 991, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 86 terms through n = 4333 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A098699_first = NULL;
static uint64_t A098699_nmax = 0;

static void A098699_build(uint64_t nmax) {
    uint64_t L = nmax * nmax / 4 + 4;
    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint64_t *d = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    uint64_t *first = (uint64_t *)calloc((size_t)(nmax + 1), sizeof(uint64_t));
    for (uint64_t i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    d[0] = 0;
    if (L >= 1) d[1] = 0;
    for (uint64_t x = 2; x <= L; x++) {
        uint64_t p = spf[x];
        uint64_t m = x / p;
        d[x] = d[m] * p + m;
        if (d[x] <= nmax && first[d[x]] == 0) first[d[x]] = x;
    }
    free(spf);
    free(d);
    if (A098699_first) free(A098699_first);
    A098699_first = first;
    A098699_nmax = nmax;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    uint64_t nn = (uint64_t)n;
    if (A098699_first == NULL || nn > A098699_nmax) {
        uint64_t nm = A098699_nmax ? A098699_nmax * 2 : 64;
        if (nm < nn) nm = nn;
        A098699_build(nm);
    }
    return (seq_t)A098699_first[nn];
}
