/*
 * A108202  a(2n) = A007376(n); a(2n+1) = a(n).
 * https://oeis.org/A108202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   * a(0) = 0 * a(2*n) = A007376(n) // digit of natural numbers
 *            written in order * a(2*n+1) = a(n) * * Small helper: A007376(k)
 *            returns the k‑th digit (0‑indexed) of *
 *            0,1,2,3,4,5,6,7,8,9,10,11,
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   A007376
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A007376.h"

seq_t a(idx_t n)
{
    /* Reduce odd indices by repeatedly dividing by 2 until even (or zero). */
    while (n & 1)
        n >>= 1;

    /* For even n (or zero) return the digit at position n/2. */
    return A007376(n / 2);
}
