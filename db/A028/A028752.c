/*
 * A028752  Nonsquares mod 39.
 * https://oeis.org/A028752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 39 * floor((n-1)/25) + r[(n-1)%25] * where r contains the
 *            sorted list of nonsquare residues modulo 39
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* idx_t is defined elsewhere */

static const idx_t nonres[25] = {
    2, 5, 6, 7, 8,
    11, 14, 15, 17, 18,
    19, 20, 21, 23, 24,
    26, 28, 29, 31, 32,
    33, 34, 35, 37, 38
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for invalid input */

    idx_t idx0    = n - 1;            /* zero‑based index of the term */
    idx_t block   = idx0 / 25;        /* number of complete 39‑blocks */
    idx_t rel     = idx0 % 25;        /* position within the block */

    seq_t base = (seq_t)block * 39;   /* contribution from full blocks */
    return base + (seq_t)nonres[rel];
}
