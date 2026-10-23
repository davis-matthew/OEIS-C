/*
 * A037829  Number of i such that d(i) >= d(i-1), where Sum_{i=0..m} d(i)*6^i
 *          is the base-6 representation of n.
 * https://oeis.org/A037829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = #\,\{i\ge 1 : d(i)\ge d(i-1)\}, where n = Σ_{i=0}^{m}
 *            d(i)*6^i is the base‑6 expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes the number of positions i (i≥1) in the base‑6 representation of
   n where the digit d(i) is greater than or equal to the previous digit
   d(i‑1).  For n = 0 the function returns 0, but the OEIS sequence is
   defined only for n ≥ 1.  The implementation is O(log₆ n) and uses only
   integer operations. */
seq_t a(idx_t n)
{
    /* For single‑digit numbers (n<6) the answer is always 0. */
    if (n < 6u)
        return 0;

    /* Previous digit (least significant). */
    idx_t prev = n % 6u;
    n /= 6u;

    idx_t count = 0;
    while (n > 0) {
        idx_t cur = n % 6u;
        if (cur >= prev)
            ++count;
        prev = cur;
        n /= 6u;
    }

    return (seq_t)count;
}
