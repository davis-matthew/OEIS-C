/*
 * A028762  Nonsquares mod 49.
 * https://oeis.org/A028762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 49*floor((n‑1)/27) + arr[(n‑1)%27] where arr is the *
 *            list of residues mod 49 that are NOT quadratic residues modulo
 *            49. * arr contains the 27 residues in ascending order:
 *            3,5,6,7,10,12,13, *
 *            14,17,19,20,21,24,26,27,28,31,33,34,35,38,40,41,42,45,47,48
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t ns_mod49[27] = {
    3,5,6,7,10,12,13,14,17,19,20,21,24,26,27,28,31,33,34,35,38,40,41,42,45,47,48
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; n==0 is undefined */
    idx_t m = n - 1;                      /* zero‑based index */
    idx_t q = m / 27;                     /* number of complete 49‑blocks */
    idx_t r = m % 27;                     /* position within the block */
    return (seq_t)(q * 49 + ns_mod49[r]); /* assemble the result */
}
