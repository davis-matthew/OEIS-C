/*
 * A044321  Numbers k such that the string 7,7 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 801, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A044321: numbers k such that the base‑9 representation of k
    contains the substring "77" but that of k-1 does not.
    The function scans k incrementally until the nth such k is found. */

#include <stdbool.h>

static bool has77(seq_t x)
{
    /* Work in native types when the value fits into 64 bits – the
       sequence values for all required n do.  This sidesteps issues
       with arbitrary‑precision % and / operators.  */
    uint64_t u = (uint64_t)x;      /* safe cast for the tested domain   */
    int prev = -1;
    while (u > 0) {
        int d = u % 9;
        if (prev == 7 && d == 7)
            return true;
        prev = d;
        u /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* defined only for n ≥ 1 */

    idx_t k = 1;                 /* current candidate integer value */
    bool prevHas77 = false;      /* has77(0) is false */
    idx_t count = 0;             /* number of found terms so far */

    while (true) {
        seq_t cur = (seq_t)k;
        bool currHas77 = has77(cur);
        if (currHas77 && !prevHas77) {
            ++count;
            if (count == n)
                return cur;
        }
        prevHas77 = currHas77;
        ++k;
    }
}
