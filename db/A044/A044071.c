/*
 * A044071  Numbers n such that string 2,2 occurs in the base 4 representation
 *          of n but not of n-1.
 * https://oeis.org/A044071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Find the n-th integer n whose base‑4 representation
   contains the consecutive digits 22, while (n–1) does not.
   The function scans integers sequentially and checks the
   condition on each candidate. */

#include <stdbool.h>

/* Returns true if v (in base 4) contains at least one occurrence
   of the consecutive digits 2,2. */
static bool has22(seq_t v)
{
    if (v == 0)
        return false;

    int prev = -1;
    while (v > 0) {
        int digit = (int)(v % 4);   /* 0 <= digit <= 3 */
        if (digit == 2 && prev == 2)
            return true;
        prev = digit;
        v = v / 4;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; define n==0 as a safety value */
        return 0;

    idx_t found = 0;      /* number of terms found so far */
    seq_t candidate = 1;  /* current integer to test */

    while (1) {
        if (has22(candidate) && !has22(candidate - 1)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
}
