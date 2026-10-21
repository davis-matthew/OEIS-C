/*
 * A063454  Number of solutions to x^3 + y^3 = z^3 mod n.
 * https://oeis.org/A063454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   Number of solutions to x^3 + y^3 = z^3 (mod n), i.e. number of
 *            (x,y,z) with 0 ≤ x,y,z < n and x^3 + y^3 ≡ z^3 (mod n)
 * Range:     u8 n <= 15, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* should never happen, but keep safe */
    /* Allocate arrays for residue counts */
    uint64_t *f = (uint64_t*)malloc((size_t)n * sizeof(uint64_t));
    uint64_t *g = (uint64_t*)malloc((size_t)n * sizeof(uint64_t));
    if (!f || !g) {                 /* allocation failed */
        free(f); free(g);
        return 0;
    }
    memset(f, 0, (size_t)n * sizeof(uint64_t));
    memset(g, 0, (size_t)n * sizeof(uint64_t));

    /* Count cube residues modulo n */
    for (idx_t x = 0; x < n; ++x) {
        idx_t r = (x * x) % n;    /* x^2 mod n */
        r = (r * x) % n;          /* x^3 mod n */
        f[r]++;
    }

    /* Compute convolution f * f (circular, modulo n) */
    for (idx_t i = 0; i < n; ++i) {
        uint64_t fi = f[i];
        if (!fi) continue;
        for (idx_t j = 0; j < n; ++j) {
            uint64_t fj = f[j];
            if (!fj) continue;
            idx_t s = i + j;
            if (s >= n) s -= n;          /* wrap modulo n */
            g[s] += fi * fj;              /* fi*fj ≤ n^2 < 2^32 for n ≤ 1e4 */
        }
    }

    /* Final count: sum over s of g[s] * f[s] */
    uint64_t result = 0;
    for (idx_t s = 0; s < n; ++s) {
        result += g[s] * f[s];            /* product ≤ n^3 < 2^40 for n ≤ 1e4 */
    }

    free(f);
    free(g);
    return (seq_t)result;
}
