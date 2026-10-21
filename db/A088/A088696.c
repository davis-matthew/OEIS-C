/*
 * A088696  Triangle read by rows, giving number of partial quotients in
 *          continued fraction representation of terms in the left branch of
 *          the infinite Stern-Brocot tree.
 * https://oeis.org/A088696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088696: number of partial quotients in continued‑fraction
   representations of the left branch fractions of the Stern–Brocot tree.
   Formula: Let r = ⌊log₂(n)⌋, p = n - 2ʳ.
   Then a(n) = 1 + popcount( p ⊕ (p >> 1) )   (p⊕= XOR, >> = right shift,
   popcount = number of set bits).  For n ≥ 1.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine r = floor(log2(n)) */
    idx_t r = 0;
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        r++;
    }

    /* base = 2^r */
    idx_t base = ((idx_t)1) << r;

    /* p = n - 2^r, 0 ≤ p < 2^r */
    idx_t p = n - base;

    /* Gray code of p */
    idx_t gray = p ^ (p >> 1);

    /* Count bits set in gray */
    idx_t cnt = 0;
    while (gray) {
        gray &= gray - 1;  /* clear lowest set bit */
        cnt++;
    }

    return (seq_t)(1 + cnt);
}
