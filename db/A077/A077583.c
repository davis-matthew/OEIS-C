/*
 * A077583  Triangle read by rows in which the n-th row contains n numbers
 *          starting with n and not coprime to n, except the first row is {1}.
 * https://oeis.org/A077583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by generating the triangle row for the * row r
 *            such that r(r+1)/2 >= n. For r > 1 the row consists of the *
 *            first r integers >= r that are not coprime with r. The nth term
 *            is * the (n - r(r-1)/2 - 1)-th element of that row. Row 1 is the
 *            special * case {1}
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* locate the row r that contains index n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    idx_t prev = r * (r - 1) / 2;            /* number of elements before row r */
    idx_t offset = n - prev - 1;             /* 0‑based position inside row r */

    /* walk through the row until the desired element is found */
    idx_t count = 0;
    idx_t cur   = r;
    seq_t result = 0;
    while (1) {
        if (gcd_idx(cur, r) > 1) {
            if (count == offset) {
                result = (seq_t)cur;
                break;
            }
            ++count;
        }
        ++cur;
    }
    return result;
}
