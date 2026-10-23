/*
 * A034050  Numbers with multiplicative digital root value 3.
 * https://oeis.org/A034050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 55, u64 n <= 209, u128 n <= 780,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A034050 (Numbers with one ‘3’ and any number of ‘1’s).
   Formula: a(m*(m-1)/2 + j + 1) = (10^m - 1) / 9 + 2 * 10^j   for 0 ≤ j < m.
   Here m is the number of digits of the term and j is the 0‑based position of the lone “3”.
   The implementation builds the digits directly to avoid overflow on small seq_t types. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1, but guard against 0 just in case */
        return 0;

    /* Find the smallest m with m*(m+1)/2 >= n.  For n up to 10 000 m ≤ 141. */
    idx_t m = 1;
    while (m * (m + 1) / 2 < n)
        ++m;

    /* Starting index of terms having m digits */
    idx_t n_start = (m - 1) * m / 2 + 1;
    idx_t j = n - n_start;          /* 0‑based position of the digit ‘3’   */

    /* Build the number of m ones: 111…1 (m digits) */
    seq_t all_ones = 0;
    for (idx_t i = 0; i < m; ++i) {
        all_ones = all_ones * (seq_t)10 + (seq_t)1;
    }

    /* Compute 10^j (j < m, so j ≤ 140 for the target range). */
    seq_t pow10j = 1;
    for (idx_t i = 0; i < j; ++i) {
        pow10j *= (seq_t)10;
    }

    /* Final term: the lone 3 is added as 2 * 10^j. */
    return all_ones + (seq_t)2 * pow10j;
}
