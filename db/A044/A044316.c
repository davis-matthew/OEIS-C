/*
 * A044316  Numbers n such that string 7,2 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044316: Numbers n such that the base‑9 representation of n
 * contains the contiguous digits "72" while the base‑9 representation of
 * n − 1 does not.  The function a(n) returns the n‑th such number
 * (offset 1).  It is computed by a straightforward linear scan. */
 
#include <stdbool.h>
 
/* checks whether the base‑9 representation of x contains the substring 7,2 */
static bool has_72(seq_t x)
{
    int prev = -1;                       /* previous least‑significant digit */
    while (x != 0) {
        int cur = (int)(x % 9);          /* current digit */
        if (prev == 2 && cur == 7)       /* found 7 (higher) followed by 2 */
            return true;
        prev = cur;
        x /= 9;
    }
    return false;
}
 
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1; safety */
    idx_t count = 0;
    seq_t m = 1;                         /* numbers are positive */
    while (1) {
        if (has_72(m) && !has_72(m - 1)) {
            ++count;
            if (count == n)
                return m;
        }
        ++m;
    }
}
