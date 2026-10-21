/*
 * A078112  Coefficients a(n) in the unique expansion sin(1) = Sum[a(n)/n!,
 *          n>=1], where a(n) satisfies 0<=a(n)<n.
 * https://oeis.org/A078112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n ≡ 0 or 1 (mod 4); a(n) = n‑1 if n ≡ 2 or 3 (mod 4)
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for n < 1 as a safe default (sequence is defined for n ≥ 1). */
    if (n == 0)
        return 0;

    /* Fast mod‑4 test using bitwise AND. */
    if ((n & 3u) <= 1u)   /* n%4 == 0 or 1 */
        return 0;

    /* n%4 == 2 or 3 */
    return (seq_t)(n - 1);
}
