/*
 * A044271  Numbers n such that string 2,2 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑9 representation
 *            contains the substring "22" while that of n‑1 does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has22(seq_t x)
{
    const seq_t nine = 9u;          /* base 9 for digit extraction */
    seq_t last = nine;              /* a value that cannot be a digit (0–8) */
    while (x > 0) {
        seq_t d = x % nine;
        if (d == 2 && last == 2)
            return true;
        last = d;
        x /= nine;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for index 0, return 0 */

    seq_t cur = 1;                   /* candidate numbers start from 1 */
    idx_t count = 0;                 /* how many qualifying numbers found */

    while (count < n) {
        if (has22(cur) && !has22(cur - 1))
            ++count;
        ++cur;
    }

    return cur - 1;                   /* last counted qualifying number */
}
