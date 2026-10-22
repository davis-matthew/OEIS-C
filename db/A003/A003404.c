/*
 * A003404  Number of solid partitions of n supported on graph of cube.
 * https://oeis.org/A003404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: (1 + 2*q^2 + 2*q^3 + 3*q^4 + 3*q^5 + 5*q^6 + 4*q^7 + 8*q^8
 *            + 4*q^9 + 5*q^10 + 3*q^11 + 3*q^12 + 2*q^13 + 2*q^14 +
 *            q^16)/((1-q)*(1-q^2)*...*(1-q^8)); computed by expanding the
 *            numerator and dividing by each (1-q^k) via prefix sums b[i] +=
 *            b[i-k]
 * Range:     u8 n <= 10, u16 n <= 33, u32 n <= 200, u64 n <= 4989,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *vals = NULL;
static idx_t vals_len = 0;

static const int A3404_num[17] = {1, 0, 2, 2, 3, 3, 5, 4, 8, 4, 5, 3, 3, 2, 2, 0, 1};

seq_t a(idx_t n) {
    if (n < vals_len) return vals[n];

    idx_t len = n + 1;
    if (len < 64) len = 64;
    seq_t *b = (seq_t *)malloc(len * sizeof(seq_t));
    for (idx_t i = 0; i < len; i++) {
        b[i] = (seq_t)0;
        if (i < 17) b[i] += A3404_num[i];
    }
    for (idx_t k = 1; k <= 8; k++) {
        for (idx_t i = k; i < len; i++) {
            b[i] += b[i - k];
        }
    }
    if (vals) free(vals);
    vals = b;
    vals_len = len;
    return vals[n];
}
