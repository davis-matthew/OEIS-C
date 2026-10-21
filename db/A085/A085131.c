/*
 * A085131  Multiples of 8 which are 7-smooth.
 * https://oeis.org/A085131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 316, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085131: multiples of 8 that are 7‑smooth.
   Computed as 8 times the n‑th 7‑smooth number (A002473).
   Generated with the standard 4‑pointer algorithm for humble numbers. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but guard anyway */
    /* allocate array to hold the first n 7‑smooth numbers */
    seq_t *h = (seq_t*)malloc(sizeof(seq_t) * (size_t)n);
    if (!h) return 0;              /* allocation failure – not expected */

    h[0] = 1;                      /* first 7‑smooth number */
    idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

    for (idx_t i = 1; i < n; ++i) {
        seq_t v2 = h[i2] * 2;
        seq_t v3 = h[i3] * 3;
        seq_t v5 = h[i5] * 5;
        seq_t v7 = h[i7] * 7;

        /* find the minimum of the four candidates */
        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;
        if (v7 < next) next = v7;

        h[i] = next;

        if (next == v2) ++i2;
        if (next == v3) ++i3;
        if (next == v5) ++i5;
        if (next == v7) ++i7;
    }

    seq_t result = h[n - 1] * 8;    /* multiply the n‑th 7‑smooth number by 8 */
    free(h);
    return result;
}
