/*
 * A028798  Nonsquares mod 85.
 * https://oeis.org/A028798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n‑th positive integer that is NOT a quadratic
 *            residue modulo 85
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t nonsq[58];
    static bool init = false;
    if (!init) {
        /* Quadratic residues modulo 5: 0, 1, 4 */
        static const bool qr5[5] = { true, true, false, false, true };
        /* Quadratic residues modulo 17: 0, 1, 2, 4, 8, 9, 13, 15, 16 */
        static const bool qr17[17] = {
            true,  /* 0 */
            true,  /* 1 */
            true,  /* 2 */
            false, /* 3 */
            true,  /* 4 */
            false, /* 5 */
            false, /* 6 */
            false, /* 7 */
            true,  /* 8 */
            true,  /* 9 */
            false, /*10 */
            false, /*11 */
            false, /*12 */
            true,  /*13 */
            false, /*14 */
            true,  /*15 */
            true   /*16 */
        };

        unsigned count = 0;
        for (uint8_t r = 0; r < 85; ++r) {
            bool isQR = qr5[r % 5] && qr17[r % 17];
            if (!isQR) {
                nonsq[count++] = (seq_t)r;
            }
        }
        init = true;
    }

    idx_t k = n - 1;          /* zero‑based index */
    idx_t block = k / 58;     /* which full block of 85 numbers */
    idx_t pos   = k % 58;     /* position inside the block */

    return (seq_t)block * (seq_t)85 + nonsq[pos];
}
