/*
 * A044773  Numbers n such that string 6,0 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers containing the decimal substring "60" but not the next integer. */
#include <stdbool.h>

/* Return true if the decimal representation of x contains the substring "60" */
static inline bool has_60(seq_t x)
{
    if (x == 0) return false;
    seq_t prev = x % 10;  /* least significant digit */
    x /= 10;
    while (x != 0) {
        seq_t cur = x % 10;          /* next digit to the left */
        if (cur == 6 && prev == 0)   /* pair 6 followed by 0 */
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

/* a(n) returns the n-th term (n>=1) of A044773. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1 */

    seq_t cur = 0;
    idx_t count = 0;

    while (count < n) {
        ++cur;                     /* next integer to test */
        seq_t next = cur + 1;
        if (has_60(cur) && !has_60(next))
            ++count;
    }
    return cur;
}
