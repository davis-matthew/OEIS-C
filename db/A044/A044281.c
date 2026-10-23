/*
 * A044281  Numbers n such that string 3,3 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑9 representation *
 *            contains the substring "33" but whose predecessor does not. *
 *            The function computes this by scanning successive integers until
 *            the * desired term is found
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return true iff the base‑9 representation of x contains "33". */
static bool has33(const seq_t x)
{
    seq_t y = x;
    int prev = -1;
    while (y != (seq_t)0) {
        int d = (int)(y % (seq_t)9);
        if (d == 3 && prev == 3) return true;
        prev = d;
        y = y / (seq_t)9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n=0; offset is 1 */

    idx_t found = 0;
    seq_t cur = (seq_t)1;

    while (true) {
        if (has33(cur) && !has33(cur - (seq_t)1)) {
            ++found;
            if (found == n) return cur;
        }
        ++cur;
    }
}
