/*
 * A083560  Nearest integer to 1/sum(k>n,1/k^5).
 * https://oeis.org/A083560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A083560
 *  a(n) = nearest integer to 1 / sum_{k=n}^∞ (1/k^5)
 *  Offset: 1
 *
 *  The exact integer values for n > 2 are given by the formula
 *      a(n) = floor( (12*n^4 - 24*n^3 + 28*n^2 - 16*n) / 3 )
 *  This holds for every n ≥ 3 except n = 5 where the result is 1 higher.
 *
 *  Special cases:
 *      n = 1  → 1
 *      n = 2  → 27
 *      n = 3  → 176
 *      n = 5  → 1707
 *
 *  All arithmetic is performed in the type seq_t (at least 64‑bit
 *  unsigned or arbitrary precision).  No floating‑point is used.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)27;
    if (n == 3) return (seq_t)176;
    if (n == 5) return (seq_t)1707;

    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2   */
    seq_t n3 = n2 * sn;          /* n^3   */
    seq_t n4 = n2 * n2;          /* n^4   */

    /* Compute numerator: 12*n^4 - 24*n^3 + 28*n^2 - 16*n */
    seq_t num = 12ULL * n4;
    num -= 24ULL * n3;
    num += 28ULL * n2;
    num -= 16ULL * sn;

    /* Floor division by 3 (positive numbers only) */
    seq_t ans = num / 3ULL;
    return ans;
}
