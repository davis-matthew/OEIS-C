/*
 * A085132  Multiples of 9 which are 7-smooth.
 * https://oeis.org/A085132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * h(n) where h(n) is the n-th 7‑smooth number (numbers
 *            whose prime factors are ≤7). h(1)=1, h(2)=2, h(3)=3, …
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of hamming numbers (7‑smooth) */
    static seq_t *h = NULL;
    static size_t h_len = 0;
    static size_t h_cap = 0;
    static size_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

    if (n == 0) {
        return 0;          /* out of offset, not used */
    }

    /* Initialise cache on first call */
    if (!h) {
        h_cap = 16;
        h = (seq_t *)malloc(h_cap * sizeof(seq_t));
        if (!h) return 0;          /* malloc failed */
        h[0] = 0;                   /* unused slot for 1‑based access */
        h[1] = (seq_t)1;            /* first 7‑smooth number */
        h_len = 1;
        i2 = i3 = i5 = i7 = 1;
    }

    /* Extend cache up to requested index */
    while (h_len < (size_t)n) {
        seq_t next2 = h[i2] * (seq_t)2;
        seq_t next3 = h[i3] * (seq_t)3;
        seq_t next5 = h[i5] * (seq_t)5;
        seq_t next7 = h[i7] * (seq_t)7;

        /* find minimum of the candidates */
        seq_t next = next2;
        if (next3 < next) next = next3;
        if (next5 < next) next = next5;
        if (next7 < next) next = next7;

        /* advance indices whose multiple equals the chosen next number */
        if (next == next2) ++i2;
        if (next == next3) ++i3;
        if (next == next5) ++i5;
        if (next == next7) ++i7;

        /* grow storage if needed */
        if (h_len + 1 == h_cap) {
            h_cap = h_cap * 2;
            h = (seq_t *)realloc(h, h_cap * sizeof(seq_t));
        }
        h[++h_len] = next;
    }

    return h[(size_t)n] * (seq_t)9;
}
