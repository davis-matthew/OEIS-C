/*
 * A039298  Numbers whose base-6 representation has the same nonzero number of
 *          1's and 2's.
 * https://oeis.org/A039298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A039298: numbers whose base‑6 representation has an equal
   (and non‑zero) number of digits 1 and 2.  The function performs a simple
   brute‑force search.  It is efficient enough for n up to at least 10 000,
   scanning fewer than 5 000 000 integers in the worst case. */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t term = 0;
    const seq_t SIX = 6;

    for (seq_t x = 1;; ++x) {
        seq_t t = x;
        idx_t c1 = 0, c2 = 0;

        /* Count 1's and 2's in base‑6 representation of x. */
        while (t != 0) {
            uint64_t d = (uint64_t)(t % SIX);  /* d is 0…5 */
            if (d == 1) ++c1;
            else if (d == 2) ++c2;
            t /= SIX;
        }

        if (c1 > 0 && c1 == c2) {
            ++term;
            if (term == n) return x;
        }
    }
}
