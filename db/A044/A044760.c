/*
 * A044760  Numbers n such that string 4,7 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer such that its decimal representation
 *            * contains the substring "47" but (n+1) does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return true if x contains the consecutive digits 4 followed by 7. */
static inline bool has47(seq_t x)
{
    if (x < (seq_t)10) return false;          /* at least two digits needed */
    int next = (int)(x % (seq_t)10);           /* least‑significant digit */
    x /= (seq_t)10;
    while (x > (seq_t)0) {
        int cur = (int)(x % (seq_t)10);       /* next more significant digit */
        if (cur == 4 && next == 7) return true;
        next = cur;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = (seq_t)0;

    while (true) {
        x += (seq_t)1;                          /* next candidate */
        if (has47(x) && !has47(x + (seq_t)1)) {
            count += (idx_t)1;
            if (count == n) return x;
        }
    }
}
