/*
 * A045059  Numbers whose base-4 representation contains exactly four 0's and
 *          two 2's.
 * https://oeis.org/A045059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑4 representation contains exactly
 * four 0's and two 2's (OEIS A045059).
 * Computed by exhaustive enumeration of base‑4 numbers. */
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1 */

    idx_t count = 0;              /* how many valid numbers seen */

    /* Enumerate lengths starting from 6 (minimum digits required). */
    for (unsigned L = 6; ; ++L) {
        /* Power of 4: 4^(L-1) and 4^L */
        seq_t lower = ((seq_t)1) << (2 * (L - 1));
        seq_t upper = ((seq_t)1) << (2 * L);
        upper -= 1;

        /* Enumerate all L‑digit base‑4 numbers. */
        for (seq_t val = lower; val <= upper; ++val) {
            unsigned zeros = 0;
            unsigned twos  = 0;
            seq_t tmp = val;

            /* Extract base‑4 digits using bitwise operations
             * (tmp % 4 == tmp & 3, tmp / 4 == tmp >> 2 for unsigned types). */
            for (int i = 0; i < (int)L; ++i) {
                unsigned d = (unsigned)(tmp & 3);
                if (d == 0) zeros++;
                else if (d == 2) twos++;
                tmp >>= 2;
            }

            if (zeros == 4 && twos == 2) {
                if (++count == n) return val;
            }
        }
    }
}
