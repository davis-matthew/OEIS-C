/*
 * A044061  Numbers k such that string 0,0 occurs in the base 4 representation
 *          of k but not of k-1.
 * https://oeis.org/A044061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 3336, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044061: Numbers k such that the string “00” occurs in the base‑4
   representation of k but not of k‑1.
   a(n) = 16 * m, where m is the n‑th positive integer for which (m‑1) in base‑4
   contains no consecutive zeros. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n < 1, safe guard */

    idx_t count = 0;
    for (idx_t m = 1; ; ++m)
    {
        /* test if (m‑1) contains the substring “00” in base‑4 */
        idx_t x = m - 1;
        bool has00 = false;
        int prev = -1;          /* cannot be 0, 1, 2 or 3 */
        while (x > 0)
        {
            int curr = x % 4;
            if (prev == 0 && curr == 0)
            {
                has00 = true;
                break;
            }
            prev = curr;
            x /= 4;
        }
        if (!has00)
        {
            ++count;
            if (count == n)
                return (seq_t)16 * (seq_t)m;
        }
    }
}
