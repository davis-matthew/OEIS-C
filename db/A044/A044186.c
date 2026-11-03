/*
 * A044186  Numbers k such that string 6,6 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k such that the base‑7 representation of k contains
   the substring 66 but the representation of k‑1 does not (search‑based). */

#include <stdbool.h>

static bool has66(seq_t v)
{
    if (v == 0)            /* 0 has no digits 6 */
        return false;
    int prev = -1;         /* previous digit, -1 indicates none */
    seq_t x = v;
    while (x != 0) {
        int d = (int)(x % (seq_t)7);   /* least significant digit in base 7 */
        if (d == 6 && prev == 6)       /* found consecutive 6,6 */
            return true;
        prev = d;
        x /= (seq_t)7;                /* shift right in base 7 */
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for index 0, return 0 */
        return (seq_t)0;

    idx_t count = 0;
    seq_t k = 1;             /* start searching from the first positive integer */

    while (count < n) {
        if (has66(k) && !has66(k - (seq_t)1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
    /* Should never reach here */
    return (seq_t)0;
}
