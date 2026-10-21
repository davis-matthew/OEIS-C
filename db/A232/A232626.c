/*
 * A232626  Degree of the algebraic number 2*sin(4*Pi/n).
 * https://oeis.org/A232626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A232626: degree of the algebraic number 2*sin(4*Pi/n).
   Formula:
       a(n) =  A093819(n)            if n is odd,
               A093819(n/2)          if n is even,
   with A093819(m) defined as:
       if m == 4:    1
       else if m odd or m ≡ 2 (mod 4):  φ(m)
       else if m ≡ 0 (mod 8):          φ(m)/2
       else (m ≡ 4 (mod 8) and m≠4):   φ(m)/4
   where φ is Euler's totient function.                               */

#include "../utils/phi.h"

seq_t a(idx_t n) {
    if (n < 1U) return 0;                /* safety, unused for n>=1 */

    /* choose argument for A093819 depending on parity of n */
    idx_t m = (n & 1U) ? n : (n >> 1);    /* m = n if n odd, else n/2 */

    /* special case m == 4 */
    if (m == 4U)
        return (seq_t)1;

    /* odd m or m ≡ 2 (mod 4) */
    if ((m & 1U) || ((m & 3U) == 2U))
        return phi(m);

    /* m ≡ 0 (mod 8) */
    if ((m & 7U) == 0U)
        return phi(m) / 2;

    /* remaining case: m ≡ 4 (mod 8) with m≠4 */
    return phi(m) / 4;
}
