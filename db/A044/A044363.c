/*
 * A044363  Numbers n such that string 3,1 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044363:
   numbers n such that the decimal representation of n contains the
   substring "31" but the representation of n‑1 does not.
   The function generates the nth term of this sequence. */

#include <stdbool.h>

static bool has31(seq_t x)
{
    seq_t prev = 10;             /* a value outside 0–9 */
    seq_t temp = x;
    while (temp > 0) {
        seq_t d = temp % 10;
        temp /= 10;
        if (prev == 1 && d == 3)  /* reversed digits: 1 followed by 3 in LSD order */
            return true;
        prev = d;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* safety for out‑of‑range indices */
        return 0;

    seq_t cur = 1;
    idx_t count = 0;

    while (count < n) {
        if (has31(cur) && !has31(cur - 1)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
    return 0;  /* unreachable */
}
