/*
 * A044142  Numbers k such that string 0,4 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer k such that the base‑7 representation
 *            of k contains the substring “04” but the base‑7 representation
 *            of k‑1 does not
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a base‑7 representation of x contains the substring
   "04".  The check is performed from least significant to most
   significant digit.  Because the digits are examined in reverse
   order, the pattern "04" in the normal representation corresponds
   to the digits 4 followed by 0 in the reversed order.  Therefore
   the condition `prev == 4 && cur == 0` indicates a hit. */
static inline bool has_sub04(seq_t x)
{
    if (x == 0)
        return false;                     /* “0” has no “04” */

    seq_t prev = (seq_t)~0;               /* invalid sentinel */
    while (x != 0) {
        seq_t cur = x % 7;
        x      /= 7;
        if (prev == 4 && cur == 0)
            return true;
        prev = cur;
    }
    return false;
}

/* Return the n‑th term of the sequence A044142.  The sequence
   is generated on the fly; no caching or recursive calls are used. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset 1, return 0 for safety */
        return 0;

    seq_t k = 0;                 /* candidate k */
    idx_t count = 0;             /* how many terms found */

    while (count < n) {
        k++;
        if (has_sub04(k) && !has_sub04(k - 1))
            count++;
    }
    return k;
}
