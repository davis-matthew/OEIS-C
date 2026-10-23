/*
 * A100661  Quet transform of A006519 (see A101387 for definition). Also,
 *          least k such that n+k has at most k ones in its binary
 *          representation.
 * https://oeis.org/A100661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n+1 is a power of two; otherwise a(n) = a(n -
 *            2^⌊log2(n)⌋ + 1) + 1. The code counts how many times the
 *            transformation n := n - 2^⌊log2(n)⌋ + 1 must be applied until n
 *            becomes of the form 2^k-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* n≥1 in the OEIS definition */

    idx_t steps = 0;
    /* n is of the form 2^k-1 iff (n & (n+1)) == 0 */
    while ((n & (n + 1)) != 0) {
        /* largest power of two ≤ n */
        idx_t pow = 1;
        while (pow <= n / 2) {
            pow <<= 1;
        }

        n = n - pow + 1;   /* apply the recurrence step */
        ++steps;
    }
    return (seq_t)(steps + 1);   /* base case contributes 1 */
}
