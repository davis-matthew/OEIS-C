/*
 * A039279  Numbers whose base-4 representation has the same nonzero number of
 *          0's and 3's.
 * https://oeis.org/A039279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose base‑4 representation contains an
 *            equal non‑zero number of 0's and 3's
 * Range:     u8 n <= 48, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation scans integers in increasing order until it has found
   n terms that satisfy the required property. */

#include <stdbool.h>

static inline bool same_zeros_threes(seq_t x)
{
    seq_t zero = 0;
    seq_t three = 0;
    while (x > 0)
    {
        seq_t d = x % 4;
        if (d == 0)
            zero++;
        else if (d == 3)
            three++;
        x = x / 4;
    }
    return (zero == three) && (zero > 0);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, but defined for safety */

    idx_t found = 0;
    seq_t x = 0;
    while (found < n)
    {
        x++;                      /* examine the next integer */
        if (same_zeros_threes(x))
            found++;
    }
    return x;                      /* x is the n-th term */
}
