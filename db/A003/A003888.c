/*
 * A003888  Degrees of irreducible representations of group L2(25).
 * https://oeis.org/A003888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   character degrees of PSL(2,q) for q = 25 (q == 1 mod 4), sorted:
 *            1; (q+1)/2 twice; q-1 repeated (q-1)/4 times; q once; q+1
 *            repeated (q-5)/4 times. Finite sequence of (q+5)/2 = 15 terms
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t q = 25;
    idx_t c1 = 1;               /* degree 1 */
    idx_t c2 = 2;               /* degree (q+1)/2 */
    idx_t c3 = (q - 1) / 4;     /* degree q-1 */
    idx_t c4 = 1;               /* degree q */
    idx_t c5 = (q - 5) / 4;     /* degree q+1 */

    if (n < 1) return (seq_t)0;
    if (n <= c1) return (seq_t)1;
    n -= c1;
    if (n <= c2) return ((seq_t)q + 1) / 2;
    n -= c2;
    if (n <= c3) return (seq_t)q - 1;
    n -= c3;
    if (n <= c4) return (seq_t)q;
    n -= c4;
    if (n <= c5) return (seq_t)q + 1;
    return (seq_t)0;
}
