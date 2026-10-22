/*
 * A001381  Weight distribution of binary [ 48,24,12 ] quadratic residue code.
 * https://oeis.org/A001381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of codewords of weight 4n in the extended binary
 *            quadratic residue code of length 48. The code is built from the
 *            cyclic shifts of the length-47 idempotent sum_{r in QR(47)} x^r,
 *            extended by an overall parity bit. All 2^24 codewords are
 *            enumerated in Gray-code order
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   a001381_pop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001381_pop.h"

static int a001381_done = 0;
static uint64_t a001381_cnt[49];

static void a001381_build(void) {
    const int p = 47;
    int isq[47];
    uint64_t piv[47];
    uint64_t basis[47];
    int nb = 0, i, s, b;
    uint64_t cur, g;

    for (i = 0; i < p; i++) { isq[i] = 0; piv[i] = 0; }
    for (i = 1; i < p; i++) isq[(i * i) % p] = 1;

    for (s = 0; s < p; s++) {
        uint64_t v = 0;
        for (i = 0; i < p; i++)
            if (isq[i]) v |= (uint64_t)1 << ((i + s) % p);
        for (b = p - 1; b >= 0; b--) {
            if ((v >> b) & 1) {
                if (piv[b]) v ^= piv[b];
                else { piv[b] = v; break; }
            }
        }
    }
    for (b = 0; b < p; b++) {
        if (piv[b]) {
            uint64_t v = piv[b];
            if (a001381_pop(v) & 1) v |= (uint64_t)1 << p; /* parity bit */
            basis[nb++] = v;
        }
    }

    for (i = 0; i <= 48; i++) a001381_cnt[i] = 0;
    cur = 0;
    a001381_cnt[0] = 1;
    for (g = 1; g < ((uint64_t)1 << nb); g++) {
        int t = 0;
        uint64_t x = g;
        while (!(x & 1)) { x >>= 1; t++; }
        cur ^= basis[t];
        a001381_cnt[a001381_pop(cur)]++;
    }
    a001381_done = 1;
}

seq_t a(idx_t n) {
    if (!a001381_done) a001381_build();
    if (n > 12) return (seq_t)0;
    return (seq_t)a001381_cnt[4 * n];
}
