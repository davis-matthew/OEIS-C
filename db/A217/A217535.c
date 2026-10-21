/*
 * A217535  Least number having in its decimal representation each digit n
 *          times.
 * https://oeis.org/A217535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1, then n zeros, then (n-1) ones, then each digit d (2 ≤
 *            d ≤ 9) repeated n times. This construction yields the minimal
 *            decimal number that contains every decimal digit exactly n times
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* a(0) is undefined for the sequence (offset 1),
       but we return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    seq_t res = 1;                 /* leading digit */

    /* add n zeros: multiply by 10^n */
    res = res * pow10(n);

    /* add (n‑1) ones */
    for (idx_t i = 0; i + 1 < n; i++)
        res = res * 10 + 1;

    /* for each digit 2‑9 add n copies */
    for (int d = 2; d <= 9; d++)
        for (idx_t i = 0; i < n; i++)
            res = res * 10 + d;

    return res;
}
