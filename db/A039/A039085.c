/*
 * A039085  Numbers whose base-9 representation has the same number of 1's and
 *          3's.
 * https://oeis.org/A039085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9749 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑9 representation has equal number of 1's and 3's */

#include <stdbool.h>

/* Returns true if x has the same count of digits 1 and 3 in base‑9.
   Zero has 0 and 0, which is equal. */
static bool equal13(seq_t x)
{
    unsigned long long c1 = 0, c3 = 0;
    if (x == 0)            /* 0 has 0 1's and 0 3's – equal */
        return true;

    seq_t t = x;
    while (t > 0) {
        seq_t d = t % 9;  /* current base‑9 digit (seq_t keeps the type) */
        if (d == 1) ++c1;
        else if (d == 3) ++c3;
        t /= 9;
    }
    return c1 == c3;
}

/* OEIS sequence A039085: numbers whose base‑9 representation has equal
   number of 1’s and 3’s.  a(1) = 0, a(2) = 2, a(3) = 4, ...   */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1: a(0) is undefined but return 0 */
        return 0;

    idx_t pos = 0;       /* how many qualifying numbers we have seen */
    seq_t cand = 0;      /* current candidate number */

    for (;;) {
        if (equal13(cand)) {
            ++pos;
            if (pos == n)
                return cand;
        }
        ++cand;
    }
}
