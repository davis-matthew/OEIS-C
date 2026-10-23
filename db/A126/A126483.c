/*
 * A126483  Number of base 15 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of base‑15 n‑digit strings whose adjacent digits differ by ≤3.
    Recurrence (DP):
      f(1,d)     = 1   for all d∈{0,…,14}
      f(k,d)     = Σ_{|d-d'|≤3} f(k-1,d')   for k>1
      a(k)       = Σ_{d=0}^{14} f(k,d)
    Initial values: a(0)=1, a(1)=15, a(2)=93, … (see OEIS entry).      */

#include <stdint.h>   /* for uint8_t etc. – may be unused but harmless */

seq_t a(idx_t n)
{
    /* a(0) is the empty string */
    if (n == 0) return (seq_t)1;

    /* DP arrays for the current and previous step
       (15 digits 0‑14) */
    seq_t prev[15], curr[15];

    /* Length 1: each digit is allowed, count = 1 per digit. */
    for (int d = 0; d < 15; ++d) prev[d] = (seq_t)1;

    /* For n = 1 we already have the total. */
    if (n == 1) {
        seq_t total = (seq_t)0;
        for (int d = 0; d < 15; ++d) total += prev[d];
        return total;
    }

    /* Iterate lengths from 2 up to n */
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 15; ++d) {
            seq_t sum = (seq_t)0;
            int start = d - 3;
            if (start < 0) start = 0;
            int end = d + 3;
            if (end > 14) end = 14;
            for (int j = start; j <= end; ++j)
                sum += prev[j];
            curr[d] = sum;
        }
        /* Copy current counts to previous for the next iteration */
        for (int d = 0; d < 15; ++d) prev[d] = curr[d];
    }

    /* Sum over final DP state to get a(n) */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 15; ++d) total += prev[d];
    return total;
}
