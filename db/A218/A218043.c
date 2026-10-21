/*
 * A218043  Base 3 numbers that have digits that sum to 3.
 * https://oeis.org/A218043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 30, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   ways_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generates the n‑th base‑3 number whose digits sum to 3 in
 * increasing numeric order.  The numbers are returned as the
 * base‑10 integer consisting of the base‑3 digits (e.g. 12 → 12,
 * 21 → 21, 102 → 102).  No closed form exists, so a constructive
 * enumeration is used. */

#include <stdint.h>
#include "../utils/ways_sum.h"

/* a(idx_t n) returns the n-th element (n >= 1).  The sequence is
 * indexed exactly as OEIS: a(1)=12, a(2)=21, a(3)=102, ... */
seq_t a(idx_t n)
{
    /* Find the length L of the required number.  For length L there are
     * (L-1)*(L+2)/2 numbers with digit sum 3. */
    idx_t cum = 0;          /* cumulative count of numbers seen so far   */
    idx_t m = 0;            /* m = L-1                                 */

    while (1) {
        ++m; /* m starts from 1 (i.e. length L=2) */
        idx_t count = m * (m + 3) / 2;
        if (cum + count >= n)
            break;
        cum += count;
    }

    idx_t L = m + 1;                 /* desired length                            */
    idx_t k = n - cum;               /* 1‑based index within this length block     */
    idx_t remaining = 3;             /* remaining sum of digits to distribute      */
    seq_t val = 0;                   /* resulting number                           */

    for (idx_t pos = 0; pos < L; ++pos) {
        for (idx_t d = (pos == 0) ? 1ULL : 0ULL; d <= 2 && d <= remaining; ++d) {
            idx_t rem_len = L - pos - 1;
            idx_t rem_sum = remaining - d;
            idx_t ways = ways_sum(rem_len, rem_sum);

            if (k > ways) {
                k -= ways;          /* skip this block of numbers */
            } else {
                /* this digit is chosen */
                val = val * (seq_t)10 + (seq_t)d;
                remaining = rem_sum;
                break;
            }
        }
    }

    return val;
}
