/*
 * A176416  Fixed point of morphism 0->0PPMM00, P->0PPMM0P, M=0PPMM0M (where
 *          P=+1, M=-1).
 * https://oeis.org/A176416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if the lowest (least‑significant) non‑zero base‑7 digit
 *            * of (n+1) is 1 or 6; +1 if it is 2 or 3; -1 if it is 4 or 5
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n + 1;          /* (n+1)  */
    /* Find the least‑significant non‑zero digit in base 7 */
    while ((m % 7) == 0)
        m /= 7;
    int d = m % 7;             /* 1 … 6 */
    static const seq_t tbl[7] = {0, 0, 1, 1, -1, -1, 0};
    return tbl[d];
}
