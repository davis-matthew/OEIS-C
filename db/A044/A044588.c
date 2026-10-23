/*
 * A044588  Numbers n such that string 2,4 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044588
   a(n) is the nth integer whose base‑8 representation contains
   the substring "24" but whose successor n+1 does not. */

#include <stdbool.h>

/* Check whether seq_t x has the substring "24" in base‑8. */
static bool has24(seq_t x)
{
    seq_t prev = 0;                 /* previous (less‑significant) digit */
    while (x > 0)
    {
        seq_t curr = x % (seq_t)8;
        if (prev == 4 && curr == 2)   /* see 24 from most‑significant to least */
            return true;
        prev = curr;
        x = x / (seq_t)8;
    }
    return false;
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t num   = (seq_t)1;

    while (found < n)
    {
        if (has24(num) && !has24(num + (seq_t)1))
        {
            ++found;
            if (found == n)
                return num;
        }
        ++num;
    }
    /* Should never reach here, but avoid compiler warning. */
    return (seq_t)0;
}
