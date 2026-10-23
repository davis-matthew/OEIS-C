/*
 * A044404  Numbers n such that string 7,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer that contains the substring "72" in its
 *            decimal representation, while its predecessor n-1 does not. This
 *            implementation sequentially scans the integers and counts
 *            occurrences until the n-th one is found
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x contains the adjacent digits 7 followed by 2. */
static bool has_72(seq_t x)
{
    if (x < 10)          /* single digit numbers cannot contain "72" */
        return false;

    seq_t last = x % 10; /* least significant digit */
    x /= 10;
    while (x > 0) {
        seq_t cur = x % 10;               /* next digit */
        if (cur == 7 && last == 2)        /* pattern "72" found */
            return true;
        last = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t found = 0;   /* how many suitable numbers we've seen */
    seq_t num   = 0;   /* current candidate */

    /* Search until we've found the n-th term.  The loop will
       terminate because the pattern occurs infinitely often. */
    for (;;) {
        ++num;                           /* test next integer */
        if (has_72(num) && !has_72(num - 1)) {
            ++found;
            if (found == n)
                return num;
        }
    }
}
