/*
 * A007970  Rhombic numbers.
 * https://oeis.org/A007970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: d (nonsquare) is a term iff the fundamental solution of
 *            x^2 - d*y^2 = 1 has odd y; y's parity is tracked through the
 *            continued fraction of sqrt(d) (convergent index r-1 if the
 *            period r is even, 2r-1 if r is odd)
 * Range:     u8 n <= 77, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t isqrt_u64(uint64_t x) {
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) { x -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
        bit >>= 2;
    }
    return r;
}

static bool is_rhombic(uint64_t d) {
    uint64_t a0 = isqrt_u64(d);
    if (a0 * a0 == d) return false;
    uint64_t m = 0, den = 1, ai = a0;
    /* q_{-1} = 0, q_0 = 1 (parities) */
    unsigned qm2 = 0, qm1 = 1;
    uint64_t r = 0, target = 0;
    uint64_t i;
    for (i = 1;; i++) {
        m = den * ai - m;
        den = (d - m * m) / den;
        ai = (a0 + m) / den;
        if (r == 0 && ai == 2 * a0) {
            r = i;
            if ((r & 1) == 0) return qm1 & 1; /* q_{r-1} */
            target = 2 * r - 1;
        }
        unsigned q = (unsigned)(((ai & 1) * qm1 + qm2) & 1);
        qm2 = qm1;
        qm1 = q;
        if (r != 0 && i == target) return q & 1;
    }
}

static uint64_t *terms = NULL;
static idx_t nterms = 0, capterms = 0;
static uint64_t next_d = 2;

seq_t a(idx_t n) {
    while (nterms < n) {
        if (is_rhombic(next_d)) {
            if (nterms == capterms) {
                idx_t nc = capterms ? capterms * 2 : 1024;
                terms = (uint64_t *)realloc(terms, nc * sizeof(uint64_t));
                capterms = nc;
            }
            terms[nterms++] = next_d;
        }
        next_d++;
    }
    return (seq_t)terms[n - 1];
}
