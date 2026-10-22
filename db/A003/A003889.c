/*
 * A003889  Degrees of irreducible representations of group L2(27).
 * https://oeis.org/A003889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of irreducible characters of PSL(2,q), q = 27 (q == 3
 *            mod 4), sorted: a(1) = 1; a(2..3) = (q-1)/2; a(4..3+(q-3)/4) =
 *            q-1; next a = q; then (q-3)/4 terms q+1. Finite sequence with
 *            (q+5)/2 = 16 terms
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t q = 27;
    const idx_t m = (q - 3) / 4;   /* 6 */
    if (n == 1) return (seq_t)1;
    if (n <= 3) return (seq_t)((q - 1) / 2);
    if (n <= 3 + m) return (seq_t)(q - 1);
    if (n == 4 + m) return (seq_t)q;
    if (n <= 4 + 2 * m) return (seq_t)(q + 1);
    return (seq_t)0;
}
