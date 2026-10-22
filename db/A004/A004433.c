/*
 * A004433  Numbers that are the sum of 4 distinct nonzero squares: of form
 *          w^2+x^2+y^2+z^2 with 0<w<x<y<z.
 * https://oeis.org/A004433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   sieve — mark every w^2+x^2+y^2+z^2 with 0<w<x<y<z up to a limit,
 *            then return the n-th marked number
 * Range:     u8 n <= 159, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *terms = 0;
static uint64_t nterms = 0;

static void build(uint64_t L) {
    unsigned char *mark = (unsigned char *)calloc(L + 1, 1);
    uint64_t w, x, y, z, cnt = 0, v;
    for (w = 1; w * w + 4 + 9 + 16 <= L + 1 + 1 && w * w <= L; w++) {
        uint64_t sw = w * w;
        for (x = w + 1; sw + x * x <= L; x++) {
            uint64_t sx = sw + x * x;
            for (y = x + 1; sx + y * y <= L; y++) {
                uint64_t sy = sx + y * y;
                for (z = y + 1; sy + z * z <= L; z++)
                    mark[sy + z * z] = 1;
            }
        }
    }
    for (v = 1; v <= L; v++) if (mark[v]) cnt++;
    free(terms);
    terms = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    nterms = 0;
    for (v = 1; v <= L; v++) if (mark[v]) terms[nterms++] = v;
    free(mark);
}

seq_t a(idx_t n) {
    if (n > nterms) {
        uint64_t L = (uint64_t)n + 1000;
        for (;;) {
            build(L);
            if (nterms >= n) break;
            L *= 2;
        }
    }
    return (seq_t)terms[n - 1];
}
