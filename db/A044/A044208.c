/*
 * A044208  Numbers k such that string 2,5 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: return true if x's base–8 representation contains the substring 25 */
static inline bool has25oct(seq_t x)
{
    seq_t y = x;
    int prev = -1;                 /* digit seen in the previous, less significant position */
    while (y > 0)
    {
        int d = (int)(y % 8);      /* current least–significant digit (0–7) */
        if (prev == 5 && d == 2)   /* pattern 5 then 2 when scanned LSB→MSB */
            return true;
        prev = d;
        y /= 8;
    }
    return false;
}

/* 
 * OEIS A044208:
 * a(n) = the n-th integer k such that the octal representation of k
 * contains the substring “25” while the octal representation of k‑1
 * does not.  The sequence starts 21, 85, 149, …
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n = 0 – the offset is 1 */

    idx_t count = 0;               /* number of qualifying terms found so far */
    seq_t k = 1;                   /* current integer being tested */

    while (1)
    {
        if (has25oct(k))
        {
            seq_t km1 = k - 1;      /* k ≥ 1, so km1 is safe */
            if (!has25oct(km1))
            {
                ++count;
                if (count == n)
                    return k;
            }
        }
        ++k;                       /* proceed to the next integer */
    }
}
