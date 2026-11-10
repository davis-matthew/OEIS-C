/*
 * A209922  Position of negative values in A209661 and A209662.
 * https://oeis.org/A209922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 such that A083025(m) is odd, i.e. m has an
 *            odd number of prime factors == 1 (mod 4), counted with
 *            multiplicity (sieve-based search)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9934 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t need = (uint64_t)n;
    uint64_t limit = 4 * need + 64;
    for (;;) {
        uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
        unsigned char *par = (unsigned char *)calloc(limit + 1, 1);
        if (!spf || !par) { free(spf); free(par); return 0; }
        uint64_t count = 0;
        for (uint64_t m = 2; m <= limit; ++m) {
            if (spf[m] == 0) {
                for (uint64_t k = m; k <= limit; k += m)
                    if (spf[k] == 0) spf[k] = (uint32_t)m;
            }
            uint64_t p = spf[m];
            par[m] = par[m / p] ^ (unsigned char)((p & 3) == 1);
            if (par[m]) {
                if (++count == need) {
                    free(spf); free(par);
                    return (seq_t)m;
                }
            }
        }
        free(spf); free(par);
        limit *= 2;
    }
}
