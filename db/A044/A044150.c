/*
 * A044150  Numbers n such that string 1,5 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer m such that the base‑7 representation of
 *            m contains the substring "15" but the representation of m-1 does
 *            not. Computation: sequentially scans integers starting at 12,
 *            testing each with a base‑7 digit check
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if the base‑7 representation of x contains the substring 1 5 (i.e., a '1' followed immediately by a '5' when reading from most significant to least). */
static inline bool has_substring_15_idx(idx_t x)
{
    int prev = -1;          /* previous (more significant) digit */
    while (x > 0)
    {
        int d = x % 7;      /* current (less significant) digit */
        if (d == 1 && prev == 5)   /* pattern 1 5 from msb to lsb */
            return true;
        prev = d;
        x /= 7;
    }
    return false;
}

/* Return the n-th term of A044150 (1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 12; ; ++m)
    {
        if (has_substring_15_idx(m) && !has_substring_15_idx(m - 1))
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
