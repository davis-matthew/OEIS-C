/*
 * A043992  Numbers k such that 5 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043992: 
   a(n) = the n‑th integer k such that the base‑9 representation of k
   contains two adjacent digits 5 and 8 (in either order) but the
   representation of k+1 does not. */

/* Detect adjacency of digits 5 and 8 in base‑9 representation of x */
#include <stdbool.h>
static bool has58(seq_t x)
{
    if (x == 0) return false;
    int prev = -1;                /* previous digit (less significant)  */
    while (x > 0)
    {
        int d = (int)(x % 9);    /* least significant digit at this step */
        if ((prev == 5 && d == 8) || (prev == 8 && d == 5))
            return true;
        prev = d;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;     /* how many terms found so far */
    idx_t k = 0;         /* candidate integer */

    /* Loop until we find the n-th term */
    while (count < n)
    {
        ++k;  /* start from 1 */
        if (has58((seq_t)k) && !has58((seq_t)(k + 1)))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }

    /* The loop always returns; this is just to satisfy the compiler */
    return 0;
}
