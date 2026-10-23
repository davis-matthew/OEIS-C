/*
 * A028789  Nonsquares mod 76.
 * https://oeis.org/A028789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of positive integers that are not quadratic residues modulo 76.
   The sequence starts with 2,3,6,7,8,10,11,12,13,14,15,18,...
   The function finds the n‑th such integer. */
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t cand = 2;               /* first candidate is 2 */
    while (1) {
        /* Check whether cand is a non‑residue modulo 76.
           A number is a quadratic residue mod 76 iff
           - cand ≡ 0 or 1 (mod 4) AND
           - cand ≡ a quadratic residue mod 19.                */
        uint64_t mod4 = cand & 3ULL;             /* cand % 4 */
        if (mod4 == 2 || mod4 == 3) {            /* non‑residue mod 4 */
            ++count;
            if (count == n) return (seq_t)cand;
        } else {                                 /* mod4 is 0 or 1 */
            uint64_t mod19 = cand % 19ULL;       /* cand % 19 */
            /* residues that are squares mod 19: 0,1,4,5,6,7,9,11,16,17 */
            if (!(mod19 == 0 || mod19 == 1 || mod19 == 4 || mod19 == 5 ||
                  mod19 == 6 || mod19 == 7 || mod19 == 9 || mod19 == 11 ||
                  mod19 == 16 || mod19 == 17)) {  /* not a square mod 19 */
                ++count;
                if (count == n) return (seq_t)cand;
            }
        }
        ++cand;
    }
}
