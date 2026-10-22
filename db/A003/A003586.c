/*
 * A003586  3-smooth numbers: numbers of the form 2^i*3^j with i, j >= 0.
 * https://oeis.org/A003586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^A022328(n)*3^A022329(n); the n-th 3-smooth number is
 *            found by merging the sequences 2*a(k) and 3*a(k) in increasing
 *            order (Hamming's method), with a(1) = 1
 * Range:     u8 n <= 27, u16 n <= 94, u32 n <= 349, u64 n <= 1344,
 *            u128 n <= 5272, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t *e2 = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    idx_t *e3 = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    idx_t i2 = 0, i3 = 0, k, t;
    const double L2 = 0.69314718055994530942, L3 = 1.09861228866810969140;
    seq_t r = 1;
    e2[0] = 0; e3[0] = 0;
    for (k = 1; k < n; k++) {
        idx_t a2 = e2[i2] + 1, b2 = e3[i2];
        idx_t a3 = e2[i3], b3 = e3[i3] + 1;
        if (a2 == a3 && b2 == b3) {
            e2[k] = a2; e3[k] = b2; i2++; i3++;
        } else {
            double v2 = (double)a2 * L2 + (double)b2 * L3;
            double v3 = (double)a3 * L2 + (double)b3 * L3;
            if (v2 < v3) { e2[k] = a2; e3[k] = b2; i2++; }
            else { e2[k] = a3; e3[k] = b3; i3++; }
        }
    }
    for (t = 0; t < e2[n - 1]; t++) r *= 2;
    for (t = 0; t < e3[n - 1]; t++) r *= 3;
    free(e2);
    free(e3);
    return r;
}
