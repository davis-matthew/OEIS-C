/*
 * A044586  Numbers n such that string 2,2 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose base‑8 representation contains the
 *            substring "22" while the base‑8 representation of (n+1) does not
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x (in decimal) has consecutive octal digits 2,2. */
static bool has22(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        /* x % 8 and x / 8 work for all supported seq_t types. */
        int d = (int)(x % (seq_t)8);
        if (d == 2 && prev == 2) return true;
        prev = d;
        x /= (seq_t)8;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t found = 0;
    idx_t  count = 0;
    for (seq_t x = 1; ; ++x) {
        if (has22(x) && !has22(x + 1)) {
            ++count;
            if (count == n) return x;
        }
    }
}
