/*
 * A054239  Inverse of A054238.
 * https://oeis.org/A054239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (i + j) * (i + j + 1) / 2 + j, where i and j are obtained
 *            by de‑interleaving the bits of n (LSB‑first: even‑indexed bits
 *            of n become i, odd‑indexed bits become j)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* De‑interleave bits of n into i and j */
    seq_t i = 0, j = 0;
    idx_t m = n;
    int k = 0;  /* bit position in n */

    while (m > 0) {
        seq_t bit = m & 1;
        if (k & 1) {
            /* odd position -> bit goes to j */
            j |= bit << (k >> 1);
        } else {
            /* even position -> bit goes to i */
            i |= bit << (k >> 1);
        }
        m >>= 1;
        ++k;
    }

    /* Compute Cantor pairing of (i, j) */
    seq_t d = i + j;                /* sum of components */
    seq_t t = d * (d + 1) / 2;       /* triangular number T(d) */
    return t + j;                   /* a(n) = T(d) + j */
}
