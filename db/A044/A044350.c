/*
 * A044350  Numbers n such that string 1,8 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044350: nth positive integer that contains the decimal substring "18" but its predecessor does not. */

#include <stdbool.h>

static bool has18(seq_t x)
{
    /* Returns true if the decimal representation of x contains the substring 18. */
    while (x >= (seq_t)10) {
        seq_t last  = x % (seq_t)10;    /* current least significant digit */
        seq_t next  = (x / (seq_t)10) % (seq_t)10; /* digit to the left of last */
        if (next == (seq_t)1 && last == (seq_t)8)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* not defined for n = 0, but return 0 safely */

    idx_t count = 0;
    idx_t cand  = 1;

    while (1) {
        if (has18((seq_t)cand) && !has18((seq_t)(cand - 1))) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
