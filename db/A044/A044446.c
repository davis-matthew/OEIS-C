/*
 * A044446  Numbers k such that string 1,0 occurs in the base-4 representation
 *          of k but not of k+1.
 * https://oeis.org/A044446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for integers k such that
   the base‑4 representation of k contains the substring "10"
   (i.e. a digit 1 followed immediately by a digit 0) but
   the representation of k+1 does not.  The sequence
   a(n) is the n‑th such integer (a(1)=4). */

#include <stdbool.h>

/* Check whether the base‑4 representation of x contains the
   substring 1 followed by 0.  Scan from the least significant
   digit up; when a 0 is seen we look at the next higher digit. */
static inline bool has10(seq_t x)
{
    while (x > 0)
    {
        seq_t digit = x % 4;       /* current least significant digit */
        if (digit == 0)
        {
            seq_t higher = x / 4;  /* remaining higher part */
            if (higher > 0 && (higher % 4) == 1)
                return true;       /* pattern 1 followed by 0 found */
        }
        x /= 4;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;        /* how many terms found so far */
    seq_t k = 1;          /* start from 1, will pass through all integers */

    while (cnt < n)
    {
        if (has10(k) && !has10(k + 1))
        {
            ++cnt;
            if (cnt == n)
                return k;
        }
        ++k;
    }
    /* Should never reach here for valid n */
    return 0;
}
