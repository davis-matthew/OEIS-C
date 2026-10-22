/*
 * A003885  Degrees of irreducible representations of group L2(17).
 * https://oeis.org/A003885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   character degrees of PSL(2,q), q = 17 (q == 1 mod 4), listed in
 *            order: 1; (q+1)/2 twice; q-1 repeated (q-1)/4 times; q once; q+1
 *            repeated (q-5)/4 times. Finite sequence with 11 terms
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t q = 17;
    idx_t k1 = 1;                    /* trivial */
    idx_t k2 = k1 + 2;               /* (q+1)/2 twice */
    idx_t k3 = k2 + (q - 1) / 4;     /* q-1 */
    idx_t k4 = k3 + 1;               /* q (Steinberg) */
    idx_t k5 = k4 + (q - 5) / 4;     /* q+1 */

    if (n <= k1) return (seq_t)1;
    if (n <= k2) return ((seq_t)q + 1) / 2;
    if (n <= k3) return (seq_t)q - 1;
    if (n <= k4) return (seq_t)q;
    if (n <= k5) return (seq_t)q + 1;
    return (seq_t)0;
}
