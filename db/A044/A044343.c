/*
 * A044343  Numbers n such that string 1,1 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose decimal representation contains
 *            the substring "11" but while the preceding integer (n-1) does
 *            NOT contain that substring. This implementation scans forward
 *            from 1, checking for the pattern on each candidate
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x in decimal contains the substring "11". */
static inline bool has_11(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 10);
        if (d == 1 && prev == 1)
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)    /* behaviour for n < 1 is undefined; use 0 */
        return 0;

    seq_t cand = 1;
    idx_t cnt = 0;

    while (1) {
        if (has_11(cand) && !has_11(cand - 1)) {
            ++cnt;
            if (cnt == n)
                return cand;
        }
        cand++;
    }
}
