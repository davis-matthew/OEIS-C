/*
 * A136960  Numbers k such that k and k^2 use only the digits 0, 4, 7 and 9.
 * https://oeis.org/A136960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 0. For n ≥ 2, a(n) = (10^m – 3) · 10^z, where m and z are
 *            chosen so that the numbers 10^m – 3 with m ≥ 1 followed by z
 *            zeros appear in ascending order. (The sequence consists of 0 and
 *            all numbers of the form 10^m – 3 with arbitrary trailing zeros.)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 46, u64 n <= 121, u128 n <= 121,
 *            big any n
 * Helpers:   pow10_loop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10_loop.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                     /* a(1) = 0 */

    /* Position of the term among the non‑zero terms,
       starting at 0 for the first term 7. */
    idx_t idx0 = n - 2;

    /* Find the smallest L such that L(L+1)/2 > idx0.
       The term lies in the block of all numbers with
       a total of L digits (including trailing zeros). */
    idx_t L = 0;
    while (L * (L + 1) / 2 <= idx0)
        ++L;                           /* after the loop L ≥ 1 */

    idx_t prev = (L - 1) * L / 2;        /* count of terms before this block */
    idx_t pos  = idx0 - prev;            /* 0‑based position within the block */
    idx_t m    = pos + 1;                /* length of the non‑zero part   */
    idx_t zeros = L - m;                 /* number of trailing zeros     */

    seq_t res = pow10_loop(m);           /* 10^m */
    res -= (seq_t)3;                     /* 10^m – 3 */
    if (zeros > 0)
        res *= pow10_loop(zeros);        /* append zeros */

    return res;
}
