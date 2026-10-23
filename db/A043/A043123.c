/*
 * A043123  Numbers k such that 1 and 4 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043123: returns the nth integer k whose base‑6 representation
   contains the digits 1 and 4 adjacent (either 14 or 41), while
   k‑1 does not.  Computed by a simple linear scan. */

#include <stdbool.h>

/* Test whether a seq_t number contains the adjacent digits 1 and 4
   in base‑6, in either order. */
static bool has_adjacent_14_or_41(seq_t num)
{
    if (num == 0) return false;

    seq_t prev = num % 6;   /* least significant digit   */
    num /= 6;
    while (num > 0) {
        seq_t cur = num % 6;  /* next more significant digit */

        /* pattern 1,4 or 4,1 detected when scanning from LSB to MSB */
        if ((prev == 4 && cur == 1) || (prev == 1 && cur == 4))
            return true;

        prev = cur;
        num /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n ≥ 1; behaviour for n == 0 is undefined per OEIS offset */
    if (n == 0) return 0;

    idx_t   count = 0;      /* how many qualifying numbers found */
    seq_t   k     = 1;      /* current candidate */

    while (count < n) {
        if (has_adjacent_14_or_41(k) && !has_adjacent_14_or_41(k - 1))
            ++count;        /* found the next term */

        ++k;                /* advance to next candidate */
    }

    return k - 1;           /* k has been incremented past the nth term */
}
