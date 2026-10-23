/*
 * A039193  Numbers whose base-11 representation has the same number of 4's
 *          and 9's.
 * https://oeis.org/A039193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th non‑negative integer whose base‑11
 *            representation contains an equal number of digits 4 and 9.
 *            Membership is tested by counting these digits in base 11
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff the base‑11 representation of x has the same
   number of 4's and 9's. */
static bool balanced(seq_t x)
{
    uint64_t cnt4 = 0, cnt9 = 0;

    if (x == 0)                /* 0 counts as equal (0,0) */
        return true;

    while (x != 0) {
        idx_t d = (idx_t)(x % 11);   /* digit in base 11 */
        if (d == 4)        ++cnt4;
        else if (d == 9)   ++cnt9;
        x /= 11;
    }
    return cnt4 == cnt9;
}

/* a(n) returns the n‑th term (a(1)=0).  It scans integers
   sequentially until the desired count is reached. */
seq_t a(idx_t n)
{
    idx_t  found   = 0;
    seq_t  value   = 0;

    if (n == 0)            /* unspecified, but harmless */
        return 0;

    /* Scan until we have found the n‑th balanced number. */
    while (1) {
        if (balanced(value)) {
            ++found;
            if (found == n)
                return value;
        }
        ++value;
    }
}
