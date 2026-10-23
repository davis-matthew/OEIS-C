/*
 * A046235  Numbers whose cube is palindromic in base 6.
 * https://oeis.org/A046235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   a(1) = 0, a(2) = 1, for 3 ≤ n ≤ 13: a(n) = 6^(n‑2) + 1 a(14) =
 *            860627456 for n ≥ 15: a(n) = 6^(n‑3) + 1 (These give all numbers
 *            whose cube is a base‑6 palindrome.)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* handle the two initial isolated terms */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* special case occurring after a(13) and before a(15) */
    if (n == 14) return (seq_t)860627456ULL;

    /* usual pattern of 6^k + 1 terms */
    if (n <= 13)
        return pow_int((seq_t)6, n - 2) + (seq_t)1;
    else
        return pow_int((seq_t)6, n - 3) + (seq_t)1;
}
