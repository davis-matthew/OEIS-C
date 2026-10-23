/*
 * A119350  First differences of A118374.
 * https://oeis.org/A119350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A119350 using the piecewise
   definition of g(i) and h(i) given in the OEIS entry.  For each
   n it iteratively finds the smallest i with g(i) >= n, then
   applies the rules:
     - if n == g(i): a(n) = h(i)
     - if n >= g(i) - 2^(i-2): a(n) = 1
     - otherwise: a(n) = a(n - g(i-1))
   (the subtraction step is performed iteratively until one of the
   first two cases applies).  The algorithm runs in O(log n). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* local arrays large enough for any 64‑bit n */
    idx_t g[70];
    seq_t h[70];
    idx_t pow2[70];

    while (1) {
        /* initialise first values */
        g[1] = 1;
        h[1] = (seq_t)1;
        pow2[1] = 0;          /* unused for i = 1 */

        int i = 1;
        /* build g, h and pow2 until g[i] >= n */
        while (g[i] < n && i < 69) {
            ++i;               /* new index i */
            /* compute pow2[i] = 2^(i-2) safely */
            if (i-2 >= 64) {
                pow2[i] = UINT64_MAX;
            } else {
                pow2[i] = (idx_t)1 << (i-2);
            }
            /* compute g[i] = 2*g[i-1] + pow2[i] with overflow check */
            idx_t tmp = g[i-1];
            idx_t new_g;
            if (tmp > (UINT64_MAX >> 1)) {
                new_g = UINT64_MAX;
            } else {
                new_g = tmp << 1;
            }
            if (new_g > UINT64_MAX - pow2[i]) {
                new_g = UINT64_MAX;
            } else {
                new_g += pow2[i];
            }
            g[i] = new_g;

            /* compute h[i] */
            if (i == 2) {
                h[i] = (seq_t)4;
            } else {
                h[i] = h[i-1] + pow2[i];   /* may overflow seq_t – that is okay for
                                             values that do not fit in seq_t */
            }
        }

        /* at this point g[i] >= n */
        if (n == g[i]) {
            return h[i];
        }

        /* if n lies in the final 2^(i-2) positions before g[i] */
        if (n >= g[i] - pow2[i]) {
            return (seq_t)1;
        }

        /* otherwise recurse by reducing n */
        n = n - g[i-1];
    }
}
