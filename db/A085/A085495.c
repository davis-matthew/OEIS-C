/*
 * A085495  Maximal order of a finite subgroup of the group GL(n,Q).
 * https://oeis.org/A085495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for all n not in {2,4,6,7,8,9,10} the maximal order equals 2^n *
 *            n!. For the exceptional n the values are hard‑coded from the
 *            OEIS entry
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle exceptional values explicitly */
    switch (n) {
        case 2:  return 12;
        case 4:  return 1152;
        case 6:  return 103680;
        case 7:  return 2903040;
        case 8:  return 696729600;
        case 9:  return 1393459200;
        case 10: return 8360755200;
    }

    /* compute 2^n * n!  (factorial first, then multiply by 2^n via bit shift) */
    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i) {
        res *= (seq_t)i;
    }
    res <<= n;   /* multiply by 2^n */
    return res;
}
