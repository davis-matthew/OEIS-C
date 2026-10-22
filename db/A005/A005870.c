/*
 * A005870  Numbers represented by hexagonal close-packing.
 * https://oeis.org/A005870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   search: n is a squared distance (scaled by 3) in the hcp
 *            lattice, i.e. n = r + 2*m^2 with m >= 0, r of the form
 *            x^2+x*y+y^2, and r == 0 (mod 3) if m is even, r == 1 (mod 3) if
 *            m is odd
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static uint64_t nterms = 0;
static uint64_t built_limit = 0;

static void build(uint64_t L) {
    bool *lo = (bool *)calloc(L + 1, sizeof(bool));
    uint64_t x, y, v, m, cnt = 0;
    for (x = 0; x * x <= L; x++) {
        for (y = 0; y <= x && x * x + x * y + y * y <= L; y++) {
            lo[x * x + x * y + y * y] = true;
        }
    }
    free(terms);
    terms = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    for (v = 0; v <= L; v++) {
        bool ok = false;
        for (m = 0; 2 * m * m <= v; m++) {
            uint64_t r = v - 2 * m * m;
            if (lo[r] && r % 3 == (m & 1)) { ok = true; break; }
        }
        if (ok) terms[cnt++] = v;
    }
    nterms = cnt;
    built_limit = L;
    free(lo);
}

seq_t a(idx_t n) {
    uint64_t L = built_limit ? built_limit : 64;
    if (terms == NULL) build(L);
    while (nterms < n) {
        L *= 2;
        build(L);
    }
    return (seq_t)terms[n - 1];
}
